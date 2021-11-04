/**

    @file      Plane.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      4.11.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#include "Plane.h"
#include "builder/PlaneBuilder.h"

namespace blink
{
    Plane::Plane(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : IGeometry(logicalDevice, commandPool)
    {
    }

    Plane::~Plane() {}

    bool Plane::create(float width, float height, uint32 widthSegments, uint32 heightSegments)
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;
        std::vector<uint16> indices;

        PlaneBuilder builder;
        builder.build(vertsPos, indices, &vertsNormal, &vertsUv0);

        return uploadData(indices, vertsPos, vertsNormal, vertsUv0);
    }
} // namespace blink
