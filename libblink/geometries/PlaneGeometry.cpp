#include "PlaneGeometry.h"
#include "GeometryUtils.h"

namespace blink
{
    PlaneGeometry::PlaneGeometry(float width, float height, PlaneFaceDirection dir, int widthSegments, int heightSegments)
    {
        m_width = width;
        m_height = height;
        m_dir = dir;
        m_widthSegments = widthSegments;
        m_heightSegments = heightSegments;

        VertAttrPos3Uv2NormalList verts;
        Uint16List indis;

        switch (m_dir)
        {
        case PlaneFaceDirection::PositiveX:
            GeometryUtils::buildPlane(verts, indis, 2, 1, 0, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_PX);     // +x
            break;
        case PlaneFaceDirection::NegativeX:
            GeometryUtils::buildPlane(verts, indis, 2, 1, 0, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_NX);      // -x
            break;
        case PlaneFaceDirection::PositiveY:
            GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, height, 1.0f, -1.0f, 0.0f, widthSegments, heightSegments, VEC3_PY);     // +y
            break;
        case PlaneFaceDirection::NegativeY:
            GeometryUtils::buildPlane(verts, indis, 0, 2, 1, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_NY);      // -y
            break;
        case PlaneFaceDirection::PositiveZ:
            GeometryUtils::buildPlane(verts, indis, 0, 1, 2, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments, VEC3_PZ);      // +z
            break;
        case PlaneFaceDirection::NegativeZ:
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
