#include "PlaneGeometry.h"
#include "GeometryUtils.h"

namespace blink
{
    PlaneGeometry::PlaneGeometry(float width, float height, Facing facing, int widthSegments, int heightSegments)
    {
        m_width = width;
        m_height = height;
        m_facing = facing;
        m_widthSegments = widthSegments;
        m_heightSegments = heightSegments;

        // build vertex
        std::vector<VertexPos3Uv2Normal> verts;
        int count = 0;

        switch (m_facing)
        {
        case Facing::PositiveX:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_PX);   // +x
            break;
        case Facing::NegativeX:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_NX);   // -x
            break;
        case Facing::PositiveY:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, height, 1.0f, -1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_PY);   // +y
            break;
        case Facing::NegativeY:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_NY);   // -y
            break;
        case Facing::PositiveZ:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_PZ);   // +z
            break;
        case Facing::NegativeZ:
            count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
            GeometryUtils::buildNormal(verts, 0, count, VEC3_NZ);   // -z
            break;
        }

        uploadVertexBuffer(BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal)
            , verts.data()
            , sizeof(verts[0])*verts.size());

        // build face index
        std::vector<uint16> indis;
        GeometryUtils::buildPlaneFaceIndex(indis, 0, widthSegments, heightSegments);
        uploadIndexBuffer(indis.data(), indis.size());
    }

    PlaneGeometry::~PlaneGeometry()
    {
    }
}
