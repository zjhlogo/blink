/*!
 * \file Types.h
 *
 * \author zjhlogo
 * \date 2020/03/20
 *
 *
 */
#pragma once

#include <foundation/BaseTypesGlm.h>
#include <glm/gtx/hash.hpp>
#include <vulkan/vulkan.h>

#include <vector>

NS_BEGIN

struct VertexPosColorUv1
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    bool operator==(const VertexPosColorUv1& other) const { return pos == other.pos && color == other.color && texCoord == other.texCoord; }

    static const std::vector<VkVertexInputBindingDescription>& getBindingDescription();
    static const std::vector<VkVertexInputAttributeDescription>& getAttributeDescriptions();
};

NS_END

namespace std
{
    template <> struct hash<NS::VertexPosColorUv1>
    {
        size_t operator()(NS::VertexPosColorUv1 const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
} // namespace std
