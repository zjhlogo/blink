
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

    bool VulkanMaterial::uploadUniformDescriptorBufferInfo(UniformBinding binding, const VkDescriptorBufferInfo& bufferInfo)
    {
        int index = m_pipeline->getUniformWriteSetIndexFromBinding(binding);
        if (index != -1)
        {
            m_descriptorInfoList[index].bufferInfo = bufferInfo;
            return true;
        }

        return false;
    }

    bool VulkanMaterial::create(const tstring& filePath)
    {
        destroy();

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

        m_filePath = filePath;

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_swapchain);
        if (!m_pipeline->create(m_vertexShader, m_fragmentShader, m_polygonMode, m_topology))
        {
            return false;
        }

        m_descriptorInfoList.resize(m_pipeline->getWriteSetCount());

        // uniforms
        {
            auto juniforms = j["uniforms"];
            if (juniforms.is_array())
            {
                auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
                if (uniformBlock)
                {
                    uniformBlock->prepareBuffer();

                    for (int i = 0; i < juniforms.size(); ++i)
                    {
                        auto juniform = juniforms[i];
                        auto type = juniform["type"].get<tstring>();
                        if (type == "int")
                        {
                            auto value = juniform["value"].get<int>();
                            uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
                        }
                        else if (type == "float")
                        {
                            auto value = juniform["value"].get<float>();
                            uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
                        }
                        else if (type == "vec2")
                        {
                            auto jvec2 = juniform["value"];
                            if (jvec2.is_array() && jvec2.size() == 2)
                            {
                                auto value = glm::vec2(jvec2[0].get<float>(), jvec2[1].get<float>());
                                uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
                            }
                        }
                        else if (type == "vec3")
                        {
                            auto jvec3 = juniform["value"];
                            if (jvec3.is_array() && jvec3.size() == 3)
                            {
                                auto value = glm::vec3(jvec3[0].get<float>(), jvec3[1].get<float>(), jvec3[2].get<float>());
                                uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
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
                                uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
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
                                uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
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
                                uniformBlock->setUniformMember(juniform["name"].get<tstring>(), &value);
                            }
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
                    TextureInfo texInfo{};
                    texInfo.name = jtexture["name"].get<tstring>();
                    texInfo.path = jtexture["path"].get<tstring>();
                    m_textureInfoMap.insert({texInfo.name, texInfo});
                }
            }
        }

        loadTextures();

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

    bool VulkanMaterial::setUniform(const tstring& memberName, UniformType type, const void* data)
    {
        auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
        uniformBlock->prepareBuffer();

        switch (type)
        {
        case blink::UniformType::Int:
            return uniformBlock->setUniformMember(memberName, (int*)data);
            break;
        case blink::UniformType::Float:
            return uniformBlock->setUniformMember(memberName, (float*)data);
            break;
        case blink::UniformType::Vec2:
            return uniformBlock->setUniformMember(memberName, (glm::vec2*)data);
            break;
        case blink::UniformType::Vec3:
            return uniformBlock->setUniformMember(memberName, (glm::vec3*)data);
            break;
        case blink::UniformType::Vec4:
            return uniformBlock->setUniformMember(memberName, (glm::vec4*)data);
            break;
        case blink::UniformType::Mat3:
            return uniformBlock->setUniformMember(memberName, (glm::mat3*)data);
            break;
        case blink::UniformType::Mat4:
            return uniformBlock->setUniformMember(memberName, (glm::mat4*)data);
            break;
        }

        return false;
    }

    bool VulkanMaterial::getUniform(void* dataOut, const tstring& memberName, UniformType type)
    {
        auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
        if (!uniformBlock->isReady()) return false;

        switch (type)
        {
        case blink::UniformType::Int:
            return uniformBlock->getUniformMember((int*)dataOut, memberName);
            break;
        case blink::UniformType::Float:
            return uniformBlock->getUniformMember((float*)dataOut, memberName);
            break;
        case blink::UniformType::Vec2:
            return uniformBlock->getUniformMember((glm::vec2*)dataOut, memberName);
            break;
        case blink::UniformType::Vec3:
            return uniformBlock->getUniformMember((glm::vec3*)dataOut, memberName);
            break;
        case blink::UniformType::Vec4:
            return uniformBlock->getUniformMember((glm::vec4*)dataOut, memberName);
            break;
        case blink::UniformType::Mat3:
            return uniformBlock->getUniformMember((glm::mat3*)dataOut, memberName);
            break;
        case blink::UniformType::Mat4:
            return uniformBlock->getUniformMember((glm::mat4*)dataOut, memberName);
            break;
        }

        return false;
    }

    void VulkanMaterial::destroy()
    {
        m_descriptorInfoList.clear();

        for (auto& kvpTexInfo : m_textureInfoMap)
        {
            auto& texInfo = kvpTexInfo.second;
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

    bool VulkanMaterial::loadTextures()
    {
        auto resModule = getResModule();

        // create texture and setup descriptor
        const auto& textureWriteSetIndexMap = m_pipeline->getTextureWriteSetIndexMap();
        for (auto kvp : textureWriteSetIndexMap)
        {
            auto it = m_textureInfoMap.find(kvp.first);
            if (it == m_textureInfoMap.end()) continue;
            auto& texInfo = it->second;

            auto texture = resModule->createTexture2d(texInfo.path);
            if (!texture) texture = resModule->createTexture2d(VulkanResModule::DEFAULT_TEXTURE);
            if (!texture) continue;

            auto vulkanTexture = dynamic_cast<VulkanTexture*>(texture);
            texInfo.texture = vulkanTexture;

            auto& descriptorInfo = m_descriptorInfoList[kvp.second];
            auto& imageInfo = descriptorInfo.imageInfo;
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = vulkanTexture->getTextureImage()->getImageView();
            imageInfo.sampler = vulkanTexture->getTextureSampler();
        }

        return true;
    }

} // namespace blink
