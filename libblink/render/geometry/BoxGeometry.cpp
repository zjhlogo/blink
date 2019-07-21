#include "BoxGeometry.h"
#include "GeometryUtils.h"
#include <glad/glad.h>

namespace blink
{
    template <typename T> int buildBox(std::vector<T>& verts, std::vector<uint16>& indis, float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
    {
        int count = 0;
        int startIndex = 0;

        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PX);      // +x
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NX);      // -x
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PY);      // +y
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NY);      // -y
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, depthSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PZ);      // +z
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, heightSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NZ);      // -z
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, heightSegments);

        return startIndex + count;
    }

    BoxGeometry::BoxGeometry(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments, BufferAttributes::StockAttributes vertexAttribute)
    {
        m_width = width;
        m_height = height;
        m_depth = depth;
        m_widthSegments = widthSegments;
        m_heightSegments = heightSegments;
        m_depthSegments = depthSegments;

        std::vector<uint16> indis;

        switch (vertexAttribute)
        {
        case BufferAttributes::StockAttributes::Pos3Uv2Normal:
        {
            std::vector<VertexPos3Uv2Normal> verts;
            buildBox(verts, indis, width, height, depth, widthSegments, heightSegments, depthSegments);
            uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()), GL_STATIC_DRAW);
        }
        break;
        case BufferAttributes::StockAttributes::Pos3Uv2NormalTangent:
        {
            std::vector<VertexPos3Uv2NormalTangent> verts;
            buildBox(verts, indis, width, height, depth, widthSegments, heightSegments, depthSegments);
            GeometryUtils::buildTangent(verts, indis);
            uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()), GL_STATIC_DRAW);
        }
        break;
        default:
            assert(false && "not implement yet");
            break;
        }

        uploadIndexBuffer(indis.data(), static_cast<uint32>(indis.size()), GL_STATIC_DRAW);
    }

    BoxGeometry::~BoxGeometry()
    {

    }
}
