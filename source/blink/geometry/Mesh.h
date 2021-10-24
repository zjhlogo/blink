/*!
 * \file Mesh.h
 *
 * \author zjhlogo
 * \date 2021/10/23
 *
 *
 */
#pragma once
#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

NS_BEGIN

class VulkanLogicalDevice;
class VulkanCommandPool;
class VulkanBuffer;

class Mesh
{
public:
    Mesh(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool);
    ~Mesh();

    bool loadFromFile(const tstring& filePath);
    bool uploadData(const void* vertexData,
                    uint32 vertexDataSize,
                    const uint16* indexData,
                    uint16 numIndex,
                    const std::vector<VkVertexInputBindingDescription>& vertexBindingDesc,
                    const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDesc);

private:
    void destroy();

private:
    VulkanLogicalDevice& m_logicalDevice;
    VulkanCommandPool& m_commandPool;

    VulkanBuffer* m_vertexBuffer{};
    VulkanBuffer* m_indexBuffer{};

    // TODO: reuse the description to save memory
    const std::vector<VkVertexInputBindingDescription>* m_vertexBindingDescs{};
    const std::vector<VkVertexInputAttributeDescription>* m_vertexAttributeDescs{};
};

NS_END
