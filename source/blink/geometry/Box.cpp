/**

    @file      Box.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "Box.h"
#include "builder/PlaneBuilder.h"

namespace blink
{
    Box::Box(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : IGeometry(logicalDevice, commandPool)
    {
    }

    Box::~Box() {}

    bool Box::create(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
    {
        std::vector<glm::vec3> vertsPos;
        std::vector<glm::vec3> vertsNormal;
        std::vector<glm::vec2> vertsUv0;
        std::vector<uint16> indices;

        PlaneBuilder builder;

        // top
        builder
            .size(width, depth)
            .segment(widthSegments, depthSegments)
            .translate(glm::vec3(0.0f, 0.5f * height, 0.0f))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        // bottom
        builder
            .size(width, depth)
            .segment(widthSegments, depthSegments)
            .orient(glm::angleAxis(glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, -0.5f * height, 0.0f))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        // front
        builder
            .size(width, height)
            .segment(widthSegments, heightSegments)
            .orient(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, 0.0f, 0.5f * depth))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        // back
        builder
            .size(width, height)
            .segment(widthSegments, heightSegments)
            .orient(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)))
            .translate(glm::vec3(0.0f, 0.0f, -0.5f * depth))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        // left
        builder
            .size(height, depth)
            .segment(heightSegments, depthSegments)
            .orient(glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)))
            .translate(glm::vec3(-0.5f * width, 0.0f, 0.0f))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        // right
        builder
            .size(height, depth)
            .segment(heightSegments, depthSegments)
            .orient(glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)))
            .translate(glm::vec3(0.5f * width, 0.0f, 0.0f))
            .build(vertsPos, indices, &vertsNormal, &vertsUv0);

        return uploadData(indices, vertsPos, vertsNormal, vertsUv0);
    }
} // namespace blink
