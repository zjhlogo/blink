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

        VertAttrPos3Uv2NormalList verts;
        Uint16List indis;

        switch (m_facing)
        {
        case Facing::PositiveX:
            GeometryUtils::buildPlane(verts, indis, 2, 1, 0, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_PX);     // +x
            break;
        case Facing::NegativeX:
            GeometryUtils::buildPlane(verts, indis, 2, 1, 0, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_NX);      // -x
            break;
        case Facing::PositiveY:
            GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, height, 1.0f, -1.0f, 0.0f, widthSegments, heightSegments, VEC3_PY);     // +y
            break;
        case Facing::NegativeY:
            GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_NY);      // -y
            break;
        case Facing::PositiveZ:
            GeometryUtils::buildPlane(verts, indis, 0, 1, 2, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_PZ);      // +z
            break;
        case Facing::NegativeZ:
            GeometryUtils::buildPlane(verts, indis, 0, 1, 2, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_NZ);     // -z
            break;
        }

        uploadVertexBuffer(BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal)
            , verts.data()
            , sizeof(verts[0])*verts.size());

        uploadIndexBuffer(indis.data(), indis.size());
    }

    PlaneGeometry::~PlaneGeometry()
    {
    }
}
