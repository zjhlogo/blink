/**

    @file      Box.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      2.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "Box.h"
#include "GeometryBuilder.h"

#include <render_vulkan/Types.h>

namespace blink
{
    Box::Box(VulkanLogicalDevice& logicalDevice, VulkanCommandPool& commandPool)
        : IGeometry(logicalDevice, commandPool)
    {
    }

    Box::~Box() {}

    bool Box::create(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
    {
        // std::vector<VertexPosNormalUv1> vertices;
        // std::vector<uint16> indices;

        // float halfWidth = width * 0.5f;
        // float halfHeight = height * 0.5f;

        // vertices.push_back({ glm::vec3(-halfWidth, -halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f) });
        // vertices.push_back({ glm::vec3(-halfWidth, +halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f) });
        // vertices.push_back({ glm::vec3(+halfWidth, +halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f) });
        // vertices.push_back({ glm::vec3(+halfWidth, -halfHeight, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f) });

        // indices.emplace_back(0);
        // indices.emplace_back(1);
        // indices.emplace_back(2);
        // indices.emplace_back(2);
        // indices.emplace_back(3);
        // indices.emplace_back(0);

        // return uploadData(vertices.data(), vertices.size(), sizeof(VertexPosNormalUv1), indices.data(), indices.size());

        int count = 0;
        int startIndex = 0;

        std::vector<VertexPosNormalUv1> vertices;
        std::vector<uint16> indices;

        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_PX); // +x
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_NX); // -x
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_PY); // +y
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_NY); // -y
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_PZ); // +z
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv1(vertices, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments);
        GeometryBuilder::buildNormal(vertices, startIndex, count, VEC3_NZ); // -z
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, heightSegments);

        return uploadData(vertices.data(), vertices.size(), sizeof(VertexPosNormalUv1), indices.data(), indices.size());
    }
} // namespace blink
