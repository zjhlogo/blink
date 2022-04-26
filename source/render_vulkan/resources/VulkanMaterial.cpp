
/*********************************************************************
 * \file   VulkanMaterial.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/21/2022
 *********************************************************************/
#include "VulkanMaterial.h"
#include "../VulkanCommandBuffer.h"
#include "../VulkanImage.h"
#include "../VulkanLogicalDevice.h"
#include "../VulkanResModule.h"
#include "../VulkanSwapchain.h"
#include "../VulkanUniformBuffer.h"
#include "VulkanTexture.h"

#include <core/modules/IResModule.h>
#include <core/resources/IGeometry.h>
#include <foundation/File.h>
#include <foundation/Log.h>
#include <tinygltf/json.hpp>

namespace blink
{
    VulkanMaterial::VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanSwapchain& swapchain)
        : m_logicalDevice(logicalDevice)
        , m_swapchain(swapchain)
    {
    }

    VulkanMaterial::~VulkanMaterial()
    {
        //
        destroy();
    }

    void VulkanMaterial::bindPipeline(VulkanCommandBuffer& commandBuffer)
    {
        //
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
    }

    bool VulkanMaterial::uploadPerCameraUniforms(const VkDescriptorBufferInfo& pcubi)
    {
        int uniformWriteSetIndex = m_pipeline->getUniformWriteSetIndexFromBinding(
            VulkanPipeline::PredefineUniformBinding::PerCameraUniformBinding);
        if (uniformWriteSetIndex != -1)
        {
            m_descriptorInfoList[uniformWriteSetIndex].bufferInfo = pcubi;
            return true;
        }
        return false;
    }

    bool VulkanMaterial::uploadPerMaterialUniforms(VulkanUniformBuffer& pmub)
    {
        // upload material uniform block
        auto uniformBlock = m_pipeline->getPredefineUniformBlock(
            VulkanPipeline::PredefineUniformBinding::PerMaterialUniformBinding);

        int uniformWriteSetIndex = m_pipeline->getUniformWriteSetIndexFromBinding(
            VulkanPipeline::PredefineUniformBinding::PerMaterialUniformBinding);
        if (uniformWriteSetIndex != -1)
        {
            uniformBlock->prepareBuffer();

            int memberCount = uniformBlock->getMemberCount();
            for (int i = 0; i < memberCount; ++i)
            {
                const auto& memberName = uniformBlock->getMemberName(i);
                auto memberType = uniformBlock->getMemberType(i);

                switch (memberType)
                {
                case UniformType::Int:
                {
                    int value{};
                    getInt(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Float:
                {
                    float value{};
                    getFloat(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Vec2:
                {
                    glm::vec2 value{};
                    getVec2(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Vec3:
                {
                    glm::vec3 value{};
                    getVec3(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Vec4:
                {
                    glm::vec4 value{};
                    getVec4(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Mat3:
                {
                    glm::mat3 value{};
                    getMat3(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                case blink::UniformType::Mat4:
                {
                    glm::mat4 value{};
                    getMat4(value, memberName);
                    uniformBlock->setUniformMember(i, value);
                }
                break;
                }
            }

            pmub.appendData(uniformBlock->getBufferData(),
                            uniformBlock->getBufferSize(),
                            &m_descriptorInfoList[uniformWriteSetIndex].bufferInfo);
        }

        // upload texture sampler uniforms
        const auto& textureWriteSetIndexMap = m_pipeline->getTextureWriteSetIndexMap();
        for (auto kvp : textureWriteSetIndexMap)
        {
            auto it = m_textureInfoMap.find(kvp.first);
            if (it == m_textureInfoMap.end()) continue;
            const auto& texInfo = it->second;

            auto& descriptorInfo = m_descriptorInfoList[kvp.second];
            auto& imageInfo = descriptorInfo.imageInfo;
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = texInfo.texture->getTextureImage()->getImageView();
            imageInfo.sampler = texInfo.texture->getTextureSampler();
        }

        return true;
    }

    bool VulkanMaterial::uploadPerInstanceUniforms(const VkDescriptorBufferInfo& piubi)
    {
        int uniformWriteSetIndex = m_pipeline->getUniformWriteSetIndexFromBinding(
            VulkanPipeline::PredefineUniformBinding::PerInstanceUniformBinding);
        if (uniformWriteSetIndex != -1)
        {
            m_descriptorInfoList[uniformWriteSetIndex].bufferInfo = piubi;
            return true;
        }
        return false;
    }

    bool VulkanMaterial::updateBufferInfos(VulkanCommandBuffer& commandBuffer, VulkanGeometry* geometry)
    {
        // check validation
        auto pipelineVertexAttrs = m_pipeline->getVertexAttrFlags();
        if (!geometry->hasVertexAttrs(pipelineVertexAttrs)) return false;
        if (m_topology != geometry->getTopology()) return false;

        // set line width when nessary
        if (m_topology == PrimitiveTopology::LineList)
        {
            vkCmdSetLineWidth(commandBuffer, 1.0f);
        }

        // setup vertex buffer
        VkBuffer buffer = *geometry->getVulkanBuffer();
        for (int i = 0; i < VertexAttrs::count_; ++i)
        {
            VertexAttrs::underlying_type currVertexAttr = 1 << i;
            if (pipelineVertexAttrs.contains(currVertexAttr))
            {
                auto offset = geometry->getVertexInputOffset(currVertexAttr);
                vkCmdBindVertexBuffers(commandBuffer, i, 1, &buffer, &offset);
            }
        }

        // setup index buffer
        vkCmdBindIndexBuffer(commandBuffer, buffer, geometry->getIndicesOffset(), VK_INDEX_TYPE_UINT16);

        // bind uniforms
        return m_pipeline->bindDescriptorSets(commandBuffer, m_descriptorInfoList);
    }

    bool VulkanMaterial::create(const tstring& filePath)
    {
        destroy();

        if (!loadConfigFromFile(filePath)) return false;
        m_filePath = filePath;

        loadTextures();

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain);
        if (!m_pipeline->create(m_vertexShader, m_fragmentShader, m_polygonMode, m_topology))
        {
            return false;
        }

        m_descriptorInfoList.resize(m_pipeline->getWriteSetCount());

        return true;
    }

    bool VulkanMaterial::recreate()
    {
        if (m_pipeline)
        {
            return m_pipeline->recreate();
        }

        return true;
    }

    void VulkanMaterial::destroy()
    {
        for (auto& kvp : m_textureInfoMap)
        {
            auto& texInfo = kvp.second;
            SAFE_RELEASE(texInfo.texture);
        }
        m_textureInfoMap.clear();

        SAFE_DELETE(m_pipeline);
        m_filePath.clear();
        m_vertexShader.clear();
        m_fragmentShader.clear();
        m_polygonMode = VK_POLYGON_MODE_FILL;
        m_topology = PrimitiveTopology::TriangleList;
    }

    bool VulkanMaterial::loadConfigFromFile(const tstring& filePath)
    {
        tstring fileContent;
        if (!File::readFileIntoString(fileContent, filePath))
        {
            LOGE("Open file failed {0}", filePath);
            return false;
        }

        auto j = nlohmann::json::parse(fileContent);

        m_vertexShader = j["vertex_shader"].get<tstring>();
        m_fragmentShader = j["fragment_shader"].get<tstring>();

        // wireframe
        {
            auto jwireframe = j["wireframe"];
            if (!jwireframe.is_null())
            {
                if (jwireframe.get<bool>())
                {
                    m_polygonMode = VK_POLYGON_MODE_LINE;
                }
            }
        }

        // linelist
        {
            auto jlinelist = j["line_list"];
            if (!jlinelist.is_null())
            {
                if (jlinelist.get<bool>())
                {
                    m_topology = PrimitiveTopology::LineList;
                }
            }
        }

        // uniforms
        {
            auto juniforms = j["uniforms"];
            if (juniforms.is_array())
            {
                for (int i = 0; i < juniforms.size(); ++i)
                {
                    auto juniform = juniforms[i];
                    auto type = juniform["type"].get<tstring>();
                    if (type == "int")
                    {
                        setInt(juniform["name"].get<tstring>(), juniform["value"].get<int>());
                    }
                    else if (type == "float")
                    {
                        setFloat(juniform["name"].get<tstring>(), juniform["value"].get<float>());
                    }
                    else if (type == "vec2")
                    {
                        auto jvec2 = juniform["value"];
                        if (jvec2.is_array() && jvec2.size() == 2)
                        {
                            auto value = glm::vec2(jvec2[0].get<float>(), jvec2[1].get<float>());
                            setVec2(juniform["name"].get<tstring>(), value);
                        }
                    }
                    else if (type == "vec3")
                    {
                        auto jvec3 = juniform["value"];
                        if (jvec3.is_array() && jvec3.size() == 3)
                        {
                            auto value = glm::vec3(jvec3[0].get<float>(), jvec3[1].get<float>(), jvec3[2].get<float>());
                            setVec3(juniform["name"].get<tstring>(), value);
                        }
                    }
                    else if (type == "vec4")
                    {
                        auto jvec4 = juniform["value"];
                        if (jvec4.is_array() && jvec4.size() == 4)
                        {
                            auto value = glm::vec4(jvec4[0].get<float>(),
                                                   jvec4[1].get<float>(),
                                                   jvec4[2].get<float>(),
                                                   jvec4[3].get<float>());
                            setVec4(juniform["name"].get<tstring>(), value);
                        }
                    }
                    else if (type == "mat3")
                    {
                        auto jmat3 = juniform["value"];
                        if (jmat3.is_array() && jmat3.size() == 9)
                        {
                            auto value = glm::mat3(jmat3[0].get<float>(),
                                                   jmat3[1].get<float>(),
                                                   jmat3[2].get<float>(),
                                                   jmat3[3].get<float>(),
                                                   jmat3[4].get<float>(),
                                                   jmat3[5].get<float>(),
                                                   jmat3[6].get<float>(),
                                                   jmat3[7].get<float>(),
                                                   jmat3[8].get<float>());
                            setMat3(juniform["name"].get<tstring>(), value);
                        }
                    }
                    else if (type == "mat4")
                    {
                        auto jmat4 = juniform["value"];
                        if (jmat4.is_array() && jmat4.size() == 16)
                        {
                            auto value = glm::mat4(jmat4[0].get<float>(),
                                                   jmat4[1].get<float>(),
                                                   jmat4[2].get<float>(),
                                                   jmat4[3].get<float>(),
                                                   jmat4[4].get<float>(),
                                                   jmat4[5].get<float>(),
                                                   jmat4[6].get<float>(),
                                                   jmat4[7].get<float>(),
                                                   jmat4[8].get<float>(),
                                                   jmat4[9].get<float>(),
                                                   jmat4[10].get<float>(),
                                                   jmat4[11].get<float>(),
                                                   jmat4[12].get<float>(),
                                                   jmat4[13].get<float>(),
                                                   jmat4[14].get<float>(),
                                                   jmat4[15].get<float>());
                            setMat4(juniform["name"].get<tstring>(), value);
                        }
                    }
                }
            }
        }

        // textures
        {
            auto jtextures = j["textures"];
            if (jtextures.is_array())
            {
                for (int i = 0; i < jtextures.size(); ++i)
                {
                    auto jtexture = jtextures[i];
                    TextureInfo texInfo;
                    texInfo.binding = jtexture["binding"].get<int>();
                    texInfo.path = jtexture["path"].get<tstring>();
                    texInfo.texture = nullptr;
                    m_textureInfoMap.insert({texInfo.binding, texInfo});
                }
            }
        }

        return true;
    }

    bool VulkanMaterial::loadTextures()
    {
        auto resModule = getResModule();

        for (auto& kvp : m_textureInfoMap)
        {
            auto& texInfo = kvp.second;
            if (texInfo.texture != nullptr) continue;

            auto texture = resModule->createTexture2d(texInfo.path);
            if (!texture) texture = resModule->createTexture2d(VulkanResModule::DEFAULT_TEXTURE);
            if (!texture) continue;

            auto vulkanTexture = dynamic_cast<VulkanTexture*>(texture);
            texInfo.texture = vulkanTexture;
        }

        return true;
    }

} // namespace blink
