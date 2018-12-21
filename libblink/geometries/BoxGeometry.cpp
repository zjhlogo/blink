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

        VertAttrPos3Uv2NormalList verts;
        Uint16List indis;

        GeometryUtils::buildPlane(verts, indis, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments, VEC3_PX);       // +x
        GeometryUtils::buildPlane(verts, indis, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments, VEC3_NX);       // -x

        GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments, VEC3_PY);        // +y
        GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments, VEC3_NY);        // -y

        GeometryUtils::buildPlane(verts, indis, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments, VEC3_PZ);        // +z
        GeometryUtils::buildPlane(verts, indis, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments, VEC3_NZ);      // -z

        uploadVertexBuffer(BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal)
            , verts.data()
            , sizeof(verts[0])*verts.size());

        uploadIndexBuffer(indis.data(), indis.size());
    }

    BoxGeometry::~BoxGeometry()
    {

    }
}
