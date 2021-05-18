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
#include <vulkan/vulkan.h>

#include <array>

NS_BEGIN

class Vertex
{
public:
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();
};

NS_END
