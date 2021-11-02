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
    const std::vector<VkVertexInputBindingDescription>& VertexPosNormalUv1::getBindingDescription()
    {
        static const std::vector<VkVertexInputBindingDescription> s_bindingDescription = {{0, sizeof(VertexPosNormalUv1), VK_VERTEX_INPUT_RATE_VERTEX}};
        return s_bindingDescription;
    }

    const std::vector<VkVertexInputAttributeDescription>& VertexPosNormalUv1::getAttributeDescriptions()
    {
        static const std::vector<VkVertexInputAttributeDescription> s_attributeDescriptions = {
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosNormalUv1, pos)},
            {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosNormalUv1, normal)},
            {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexPosNormalUv1, uv0)},
        };

        return s_attributeDescriptions;
    }

} // namespace blink
