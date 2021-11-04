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

        int count = 0;
        int startIndex = 0;

        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_PX); // +x
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_NX); // -x
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_PY); // +y
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_NY); // -y
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_PZ); // +z
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, heightSegments);

        startIndex += count;
        count = GeometryBuilder::buildPlaneVertexPosUv(vertsPos, vertsUv0, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments);
        vertsNormal.resize(vertsPos.size());
        GeometryBuilder::buildNormal(vertsNormal, startIndex, count, VEC3_NZ); // -z
        GeometryBuilder::buildPlaneFaceIndex(indices, startIndex, widthSegments, heightSegments);

        return uploadData(indices, vertsPos, vertsNormal, vertsUv0);
    }
} // namespace blink
