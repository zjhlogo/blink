/**

    @file      Geometry.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include "../resource/IResource.h"

#include <foundation/BaseTypesGlm.h>
#include <vulkan/vulkan.h>

#include <vector>

namespace blink
{
    class VulkanLogicalDevice;
    class VulkanCommandBuffer;
    class VulkanBuffer;
    class ResourceMgr;

    class Geometry : public IResource
    {
        friend ResourceMgr;

    public:
        uint32 getNumVertices() const { return m_numVertices; };
        uint32 getNumIndices() const { return m_numIndices; };

        bool uploadData(const std::vector<uint16>& indices,
                        const std::vector<glm::vec3>& positions,
                        const std::vector<glm::vec3>& normals,
                        const std::vector<glm::vec2>& uv0s);

        bool uploadData(const void* data,
                        VkDeviceSize dataSize,
                        uint32 numVertices,
                        uint32 numIndices,
                        VkDeviceSize offsetPosition,
                        VkDeviceSize offsetNormal,
                        VkDeviceSize offsetUv0,
                        VkDeviceSize offsetIndices);

        VkDeviceSize getVertexInputOffset(uint32 inputMask) const;
        bool hasAllVertexInputs(uint32 allInputMask) const;

        VulkanBuffer* getVulkanBuffer() const { return m_buffer; }
        VkDeviceSize getIndicesOffset() const { return m_offsetIndices; }

    protected:
        Geometry(VulkanLogicalDevice& logicalDevice);
        virtual ~Geometry();

        void destroy();

    protected:
        VulkanLogicalDevice& m_logicalDevice;

        VulkanBuffer* m_buffer{};

        uint32 m_numVertices{};
        uint32 m_numIndices{};

        VkDeviceSize m_offsetPositions{};
        VkDeviceSize m_offsetNormals{};
        VkDeviceSize m_offsetUv0s{};
        VkDeviceSize m_offsetIndices{};
    };
} // namespace blink
