#include "SphereGeometry.h"
#include "BufferAttributes.h"
#include "GeometryUtils.h"
#include <glad/glad.h>

namespace blink
{
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
        uploadIndexBuffer(indis.data(), indis.size(), GL_STATIC_DRAW);

        // setup vertex buffer
        switch (vertexAttribute)
        {
        case blink::BufferAttributes::StockAttributes::Pos3Uv2Normal:
        {
            std::vector<VertexPos3Uv2Normal> verts;
            GeometryUtils::buildSphereVertexPos3Uv2Normal(verts, radius, rings, sections);
            uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), sizeof(verts[0])*verts.size(), GL_STATIC_DRAW);
        }
        break;
        case blink::BufferAttributes::StockAttributes::Pos3Uv2NormalTangent:
        {
            std::vector<VertexPos3Uv2NormalTangent> verts;
            GeometryUtils::buildSphereVertexPos3Uv2Normal(verts, radius, rings, sections);
            GeometryUtils::buildTangent(verts, indis);
            uploadVertexBuffer(BufferAttributes::fromStock(vertexAttribute), verts.data(), sizeof(verts[0])*verts.size(), GL_STATIC_DRAW);
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
}
