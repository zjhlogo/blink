/*!
 * \file Types.cpp
 *
 * \author zjhlogo
 * \date 2020/03/20
 *
 *
 */
#include "Types.h"

namespace blink
{
    const std::vector<VkVertexInputBindingDescription>& VertexPosNormalUv0::getBindingDescription()
    {
        static const std::vector<VkVertexInputBindingDescription> s_bindingDescription = {{0, sizeof(VertexPosNormalUv0), VK_VERTEX_INPUT_RATE_VERTEX}};
        return s_bindingDescription;
    }

    const std::vector<VkVertexInputAttributeDescription>& VertexPosNormalUv0::getAttributeDescriptions()
    {
        static const std::vector<VkVertexInputAttributeDescription> s_attributeDescriptions = {
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosNormalUv0, pos)},
            {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosNormalUv0, normal)},
            {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexPosNormalUv0, uv0)},
        };

        return s_attributeDescriptions;
    }

} // namespace blink
