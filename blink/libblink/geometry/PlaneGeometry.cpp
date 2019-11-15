/*!
 * \file PlaneGeometry.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "PlaneGeometry.h"
#include "GeometryUtils.h"
#include <PredefinedVertexAttributes.h>

NS_BEGIN

template <typename T> int buildPlane(std::vector<T>& verts, float width, float height, PlaneGeometry::Facing facing, int widthSegments, int heightSegments)
{
    int count = 0;
    switch (facing)
    {
    case PlaneGeometry::Facing::PositiveX:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_PX);   // +x
        break;
    case PlaneGeometry::Facing::NegativeX:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 2, 1, 0, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_NX);   // -x
        break;
    case PlaneGeometry::Facing::PositiveY:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, height, 1.0f, -1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_PY);   // +y
        break;
    case PlaneGeometry::Facing::NegativeY:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 2, 1, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_NY);   // -y
        break;
    case PlaneGeometry::Facing::PositiveZ:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, 1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_PZ);   // +z
        break;
    case PlaneGeometry::Facing::NegativeZ:
        count = GeometryUtils::buildPlaneVertexPos3Uv2(verts, 0, 1, 2, width, height, -1.0f, 1.0f, 0.0f, widthSegments, heightSegments);
        GeometryUtils::buildNormal(verts, 0, count, VEC3_NZ);   // -z
        break;
    }

    return count;
}

PlaneGeometry::PlaneGeometry(float width, float height, Facing facing, int widthSegments, int heightSegments, BufferAttributes::StockAttributes vertexAttribute)
{
    m_width = width;
    m_height = height;
    m_facing = facing;
    m_widthSegments = widthSegments;
    m_heightSegments = heightSegments;

    // setup index buffer
    std::vector<uint16> indis;
    GeometryUtils::buildPlaneFaceIndex(indis, 0, widthSegments, heightSegments);
    uploadIndexBuffer(indis.data(), static_cast<uint32>(indis.size()));

    // setup vertex buffer
    switch (vertexAttribute)
    {
    case BufferAttributes::StockAttributes::Pos3Uv2Normal:
    {
        std::vector<VertexPos3Uv2Normal> verts;
        buildPlane(verts, width, height, facing, widthSegments, heightSegments);
        uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()));
    }
    break;
    case BufferAttributes::StockAttributes::Pos3Uv2NormalTangent:
    {
        std::vector<VertexPos3Uv2NormalTangent> verts;
        buildPlane(verts, width, height, facing, widthSegments, heightSegments);
        GeometryUtils::buildTangent(verts, indis);
        uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()));
    }
    break;
    default:
        assert(false && "not implement yet");
        break;
    }
}

PlaneGeometry::~PlaneGeometry()
{

}

NS_END
