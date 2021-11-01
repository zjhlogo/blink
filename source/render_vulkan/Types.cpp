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
    const std::vector<VkVertexInputBindingDescription>& VertexPosColorUv1::getBindingDescription()
    {
        static const std::vector<VkVertexInputBindingDescription> s_bindingDescription = {{0, sizeof(VertexPosColorUv1), VK_VERTEX_INPUT_RATE_VERTEX}};
        return s_bindingDescription;
    }

    const std::vector<VkVertexInputAttributeDescription>& VertexPosColorUv1::getAttributeDescriptions()
    {
        static const std::vector<VkVertexInputAttributeDescription> s_attributeDescriptions = {
            {0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosColorUv1, pos)},
            {1, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(VertexPosColorUv1, color)},
            {2, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(VertexPosColorUv1, texCoord)},
        };

        return s_attributeDescriptions;
    }

} // namespace blink
