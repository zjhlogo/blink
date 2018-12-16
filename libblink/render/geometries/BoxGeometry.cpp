#include "BoxGeometry.h"

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
        BufferAttributes* pBufferAttributes = BufferAttributes::fromStock(BufferAttributes::StockAttributes::Pos3Uv2Normal);

        buildPlane(verts, indis, 2, 1, 0, depth, height, -1.0f, 1.0f, 0.5f * width, depthSegments, heightSegments, VEC3_PX);       // +x
        buildPlane(verts, indis, 2, 1, 0, depth, height, 1.0f, 1.0f, -0.5f * width, depthSegments, heightSegments, VEC3_NX);       // -x

        buildPlane(verts, indis, 0, 2, 1, width, depth, 1.0f, -1.0f, 0.5f * height, widthSegments, depthSegments, VEC3_PY);        // +y
        buildPlane(verts, indis, 0, 2, 1, width, depth, 1.0f, 1.0f, -0.5f * height, widthSegments, depthSegments, VEC3_NY);        // -y

        buildPlane(verts, indis, 0, 1, 2, width, height, 1.0f, 1.0f, 0.5f * depth, widthSegments, heightSegments, VEC3_PZ);        // +z
        buildPlane(verts, indis, 0, 1, 2, width, height, -1.0f, 1.0f, -0.5f * depth, widthSegments, heightSegments, VEC3_NZ);      // -z

        uploadVertexBuffer(pBufferAttributes, verts.data(), sizeof(verts[0])*verts.size());
        uploadIndexBuffer(indis.data(), indis.size());
    }

    BoxGeometry::~BoxGeometry()
    {

    }

    void BoxGeometry::buildPlane(VertAttrPos3Uv2NormalList& verts, Uint16List& indis, int ax, int ay, int az, float width, float height, float udir, float vdir, float posZ, int segmentX, int segmentY, const glm::vec3& normal)
    {
        float divX = static_cast<float>(segmentX);
        float divY = static_cast<float>(segmentY);

        glm::vec2 uv;

        glm::vec3 vert;
        vert[az] = posZ;

        // build vertex list
        uint16 numVerts = static_cast<uint16>(verts.size());
        for (int y = 0; y < (segmentY + 1); ++y)
        {
            uv.y = y / divY - 0.5f;
            vert[ay] = uv.y * height * vdir;
            for (int x = 0; x < (segmentX + 1); ++x)
            {
                uv.x = x / divX - 0.5f;
                vert[ax] = uv.x * width * udir;

                verts.push_back({ vert.x, vert.y, vert.z, uv.x, uv.y, normal.x, normal.y, normal.z });
            }
        }

        // build face list
        for (int y = 0; y < segmentY; ++y)
        {
            for (int x = 0; x < segmentX; ++x)
            {
                uint16 i0 = y * (segmentX + 1) + x;
                uint16 i1 = i0 + 1;
                uint16 i2 = (y + 1) * (segmentX + 1) + x;
                uint16 i3 = i2 + 1;

                indis.push_back(numVerts + i0);
                indis.push_back(numVerts + i2);
                indis.push_back(numVerts + i1);

                indis.push_back(numVerts + i1);
                indis.push_back(numVerts + i2);
                indis.push_back(numVerts + i3);
            }
        }
    }
}
