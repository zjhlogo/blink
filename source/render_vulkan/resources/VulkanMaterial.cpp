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
#include "../VulkanRenderPass.h"
#include "../VulkanResourceModule.h"
#include "../VulkanUniformBuffer.h"
#include "VulkanTexture.h"

#include <core/modules/IResourceModule.h>
#include <core/resources/IGeometry.h>
#include <foundation/File.h>
#include <foundation/Log.h>
#include <foundation/PathParser.h>
#include <tinygltf/json.hpp>

#include <filesystem>

namespace blink
{
    VulkanMaterial::VulkanMaterial(VulkanLogicalDevice& logicalDevice, VulkanRenderPass& renderPass)
        : m_logicalDevice(logicalDevice)
        , m_renderPass(renderPass)
    {
        //
    }

    VulkanMaterial::~VulkanMaterial()
    {
        //
        destroy();
    }

    void VulkanMaterial::bindPipeline(const VulkanCommandBuffer& commandBuffer)
    {
        //
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *m_pipeline);
    }

    bool VulkanMaterial::updateBufferInfos(const VulkanCommandBuffer& commandBuffer, const VulkanGeometry* geometry)
    {
        // check validation
        auto pipelineVertexAttrs = m_pipeline->getVertexAttrFlags();
        if (!geometry->hasVertexAttrs(pipelineVertexAttrs))
        {
            return false;
        }
        if (m_topology != geometry->getTopology())
        {
            return false;
        }

        // set line width when necessary
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

        m_vertexShader = PathParser::getAbsolutePath(j["vertex_shader"].get<tstring>(), filePath);
        m_fragmentShader = PathParser::getAbsolutePath(j["fragment_shader"].get<tstring>(), filePath);

        // wireframe
        {
            auto jWireframe = j["wireframe"];
            if (!jWireframe.is_null())
            {
                if (jWireframe.get<bool>())
                {
                    m_polygonMode = VK_POLYGON_MODE_LINE;
                }
            }
        }

        // line-list
        {
            auto jLineList = j["line_list"];
            if (!jLineList.is_null())
            {
                if (jLineList.get<bool>())
                {
                    m_topology = PrimitiveTopology::LineList;
                }
            }
        }

        m_filePath = filePath;

        m_pipeline = new VulkanPipeline(m_logicalDevice, m_renderPass);
        if (!m_pipeline->create(m_vertexShader, m_fragmentShader, m_polygonMode, m_topology))
        {
            return false;
        }

        m_descriptorInfoList.resize(m_pipeline->getWriteSetCount());

        // uniforms
        {
            auto jUniforms = j["uniforms"];
            if (jUniforms.is_array())
            {
                auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
                if (uniformBlock)
                {
                    uniformBlock->prepareBuffer();

                    for (auto jUniform : jUniforms)
                    {
                        auto type = jUniform["type"].get<tstring>();
                        if (type == "int")
                        {
                            auto value = jUniform["value"].get<int>();
                            uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                        }
                        else if (type == "float")
                        {
                            auto value = jUniform["value"].get<float>();
                            uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                        }
                        else if (type == "vec2")
                        {
                            auto jVec2 = jUniform["value"];
                            if (jVec2.is_array() && jVec2.size() == 2)
                            {
                                auto value = glm::vec2(jVec2[0].get<float>(), jVec2[1].get<float>());
                                uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                            }
                        }
                        else if (type == "vec3")
                        {
                            auto jVec3 = jUniform["value"];
                            if (jVec3.is_array() && jVec3.size() == 3)
                            {
                                auto value = glm::vec3(jVec3[0].get<float>(), jVec3[1].get<float>(), jVec3[2].get<float>());
                                uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                            }
                        }
                        else if (type == "vec4")
                        {
                            auto jVec4 = jUniform["value"];
                            if (jVec4.is_array() && jVec4.size() == 4)
                            {
                                auto value = glm::vec4(jVec4[0].get<float>(), jVec4[1].get<float>(), jVec4[2].get<float>(), jVec4[3].get<float>());
                                uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                            }
                        }
                        else if (type == "mat3")
                        {
                            auto jMat3 = jUniform["value"];
                            if (jMat3.is_array() && jMat3.size() == 9)
                            {
                                auto value = glm::mat3(jMat3[0].get<float>(),
                                                       jMat3[1].get<float>(),
                                                       jMat3[2].get<float>(),
                                                       jMat3[3].get<float>(),
                                                       jMat3[4].get<float>(),
                                                       jMat3[5].get<float>(),
                                                       jMat3[6].get<float>(),
                                                       jMat3[7].get<float>(),
                                                       jMat3[8].get<float>());
                                uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                            }
                        }
                        else if (type == "mat4")
                        {
                            auto jMat4 = jUniform["value"];
                            if (jMat4.is_array() && jMat4.size() == 16)
                            {
                                auto value = glm::mat4(jMat4[0].get<float>(),
                                                       jMat4[1].get<float>(),
                                                       jMat4[2].get<float>(),
                                                       jMat4[3].get<float>(),
                                                       jMat4[4].get<float>(),
                                                       jMat4[5].get<float>(),
                                                       jMat4[6].get<float>(),
                                                       jMat4[7].get<float>(),
                                                       jMat4[8].get<float>(),
                                                       jMat4[9].get<float>(),
                                                       jMat4[10].get<float>(),
                                                       jMat4[11].get<float>(),
                                                       jMat4[12].get<float>(),
                                                       jMat4[13].get<float>(),
                                                       jMat4[14].get<float>(),
                                                       jMat4[15].get<float>());
                                uniformBlock->setUniformMember(jUniform["name"].get<tstring>(), &value);
                            }
                        }
                    }
                }
            }
        }

        // textures
        {
            auto jTextures = j["textures"];
            if (jTextures.is_array())
            {
                for (auto jTexture : jTextures)
                {
                    TextureInfo texInfo{};
                    texInfo.name = jTexture["name"].get<tstring>();
                    texInfo.path = PathParser::getAbsolutePath(jTexture["path"].get<tstring>(), filePath);
                    m_textureInfoMap.insert({texInfo.name, texInfo});
                }
            }
        }

        loadTextures();

        return true;
    }

    bool VulkanMaterial::setUniform(const tstring& memberName, UniformType type, const void* data)
    {
        auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
        uniformBlock->prepareBuffer();

        switch (type)
        {
        case blink::UniformType::Int:
            return uniformBlock->setUniformMember(memberName, static_cast<const int*>(data));
        case blink::UniformType::Float:
            return uniformBlock->setUniformMember(memberName, static_cast<const float*>(data));
        case blink::UniformType::Vec2:
            return uniformBlock->setUniformMember(memberName, static_cast<const glm::vec2*>(data));
        case blink::UniformType::Vec3:
            return uniformBlock->setUniformMember(memberName, static_cast<const glm::vec3*>(data));
        case blink::UniformType::Vec4:
            return uniformBlock->setUniformMember(memberName, static_cast<const glm::vec4*>(data));
        case blink::UniformType::Mat3:
            return uniformBlock->setUniformMember(memberName, static_cast<const glm::mat3*>(data));
        case blink::UniformType::Mat4:
            return uniformBlock->setUniformMember(memberName, static_cast<const glm::mat4*>(data));
        case UniformType::None:
        case UniformType::NumTypes:
            break;
        }

        return false;
    }

    bool VulkanMaterial::getUniform(void* dataOut, const tstring& memberName, UniformType type)
    {
        auto uniformBlock = m_pipeline->getUniformBlock(UniformBinding::Material);
        if (!uniformBlock->isReady())
        {
            return false;
        }

        switch (type)
        {
        case UniformType::Int:
            return uniformBlock->getUniformMember(static_cast<int*>(dataOut), memberName);
        case UniformType::Float:
            return uniformBlock->getUniformMember(static_cast<float*>(dataOut), memberName);
        case UniformType::Vec2:
            return uniformBlock->getUniformMember(static_cast<glm::vec2*>(dataOut), memberName);
        case UniformType::Vec3:
            return uniformBlock->getUniformMember(static_cast<glm::vec3*>(dataOut), memberName);
        case UniformType::Vec4:
            return uniformBlock->getUniformMember(static_cast<glm::vec4*>(dataOut), memberName);
        case UniformType::Mat3:
            return uniformBlock->getUniformMember(static_cast<glm::mat3*>(dataOut), memberName);
        case UniformType::Mat4:
            return uniformBlock->getUniformMember(static_cast<glm::mat4*>(dataOut), memberName);
        case UniformType::None:
        case UniformType::NumTypes:
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
        auto resourceModule = getResourceModule();

        // create texture and setup descriptor
        const auto& textureWriteSetIndexMap = m_pipeline->getTextureWriteSetIndexMap();
        for (const auto& kvp : textureWriteSetIndexMap)
        {
            auto it = m_textureInfoMap.find(kvp.first);
            if (it == m_textureInfoMap.end())
            {
                continue;
            }
            auto& texInfo = it->second;

            auto texture = resourceModule->createTexture2d(texInfo.path);
            if (!texture)
            {
                texture = resourceModule->createTexture2d(VulkanResourceModule::DEFAULT_TEXTURE);
            }
            if (!texture)
            {
                continue;
            }

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
