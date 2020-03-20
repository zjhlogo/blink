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
#include <vulkan/vulkan.hpp>

NS_BEGIN

class Vertex
{
public:
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static vk::VertexInputBindingDescription getBindingDescription();
    static std::array<vk::VertexInputAttributeDescription, 3> getAttributeDescriptions();
};

NS_END
