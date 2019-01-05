#include "BoxGeometry.h"
#include "GeometryUtils.h"

namespace blink
{
    BoxGeometry::BoxGeometry(float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
    {
        m_width = width;
        m_height = height;
        m_depth = depth;
        m_widthSegments = widthSegments;
        m_heightSegments = heightSegments;
        m_depthSegments = depthSegments;

        std::vector<VertexPos3Uv2Normal> verts;
        int count = 0;
        int startIndex = 0;
        std::vector<uint16> indis;

        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PX);      // +x
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, depthSegments, heightSegments);

        startIndex += count;
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NX);      // -x
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, depthSegments, heightSegments);

        startIndex += count;
        GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PY);      // +y
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, depthSegments);

        startIndex += count;
        GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NY);      // -y
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, depthSegments);

        startIndex += count;
        GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_PZ);      // +z
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, heightSegments);

        startIndex += count;
        GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, startIndex, count, VEC3_NZ);      // -z
        GeometryUtils::buildPlaneFaceIndex(indis, startIndex, widthSegments, heightSegments);

        uploadVertexBuffer(BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal)
            , verts.data()
            , sizeof(verts[0])*verts.size());

        uploadIndexBuffer(indis.data(), indis.size());
    }

    BoxGeometry::~BoxGeometry()
    {

    }
}
