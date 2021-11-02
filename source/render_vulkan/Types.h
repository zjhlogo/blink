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

namespace blink
{
    struct VertexPosNormalUv1
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec2 uv0;

        bool operator==(const VertexPosNormalUv1& other) const { return pos == other.pos && normal == other.normal && uv0 == other.uv0; }

        static const std::vector<VkVertexInputBindingDescription>& getBindingDescription();
        static const std::vector<VkVertexInputAttributeDescription>& getAttributeDescriptions();
    };

} // namespace blink

namespace std
{
    template <> struct hash<blink::VertexPosNormalUv1>
    {
        size_t operator()(blink::VertexPosNormalUv1 const& vertex) const
        {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.normal) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.uv0) << 1);
        }
    };
} // namespace std
