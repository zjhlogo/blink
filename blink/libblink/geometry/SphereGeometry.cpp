/*!
 * \file SphereGeometry.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "SphereGeometry.h"
#include "BufferAttributes.h"
#include "GeometryUtils.h"
#include <PredefinedVertexAttributes.h>

NS_BEGIN

SphereGeometry::SphereGeometry(float radius, int rings, int sections, BufferAttributes::StockAttributes vertexAttribute)
{
    if (rings < 3) rings = 3;
    if (sections < 3) sections = 3;

    m_radius = radius;
    m_rings = rings;
    m_sections = sections;

    // setup index buffer
    std::vector<uint16> indis;
    GeometryUtils::buildSphereFaceIndex(indis, rings, sections);
    uploadIndexBuffer(indis.data(), static_cast<uint32>(indis.size()));

    // setup vertex buffer
    switch (vertexAttribute)
    {
    case NS::BufferAttributes::StockAttributes::Pos3Uv2Normal:
    {
        std::vector<VertexPos3Uv2Normal> verts;
        GeometryUtils::buildSphereVertexPos3Uv2Normal(verts, radius, rings, sections);
        uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()));
    }
    break;
    case NS::BufferAttributes::StockAttributes::Pos3Uv2NormalTangent:
    {
        std::vector<VertexPos3Uv2NormalTangent> verts;
        GeometryUtils::buildSphereVertexPos3Uv2Normal(verts, radius, rings, sections);
        GeometryUtils::buildTangent(verts, indis);
        uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), static_cast<uint32>(sizeof(verts[0])*verts.size()));
    }
    break;
    default:
        assert(false && "not implement yet");
        break;
    }
}

SphereGeometry::~SphereGeometry()
{

}

NS_END
