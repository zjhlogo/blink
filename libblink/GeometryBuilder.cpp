#include "GeometryBuilder.h"
#include "GeometryUtils.h"
#include "systems/GlConfig.h"
#include <glad/glad.h>

namespace blink
{
    // pre-define vertex attributes
    class VertexPos3
    {
    public:
        float x, y, z;      // pos3
    };

    class VertexPos3Color
    {
    public:
        float x, y, z;      // pos3
        uint32 color;       // color
    };

    class VertexPos3Uv2
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
    };

    class VertexPos3Uv2Normal
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        float nx, ny, nz;   // normal
    };

    class VertexPos3Uv2Color
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        uint32 color;       // color
    };

    class VertexPos3Uv2NormalTangent
    {
    public:
        float x, y, z;      // pos3
        float u, v;         // uv2
        float nx, ny, nz;   // normal
        float tx, ty, tz;   // tangent
    };

    template <typename T> int generateBoxData(std::vector<T>& verts, std::vector<uint16>& indis, float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
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

    bool uploadVertexBuffer(BufferGeometry& bufferGeo, const void * bufferData, uint32 bufferSize)
    {
        static AttributeItem s_attrPos3Uv2Normal[] =
        {
            {3, AttributeItemType::Float, GL_FLOAT, 0, "a_position"},
            {2, AttributeItemType::Float, GL_FLOAT, 12, "a_texCoord"},
            {3, AttributeItemType::Float, GL_FLOAT, 20, "a_normal"},
        };

        // create buffer object
        if (bufferGeo.m_vertexBufferId == 0)
        {
            glGenBuffers(1, &bufferGeo.m_vertexBufferId);
            GL_ERROR_CHECK();
            if (bufferGeo.m_vertexBufferId == 0) return false;
        }

        // create vao
        if (bufferGeo.m_vertexArrayObjectId == 0)
        {
            glGenVertexArrays(1, &bufferGeo.m_vertexArrayObjectId);
            GL_ERROR_CHECK();
            if (bufferGeo.m_vertexArrayObjectId == 0) return false;
        }

        // bind vao
        glBindVertexArray(bufferGeo.m_vertexArrayObjectId);
        GL_ERROR_CHECK();

        // Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, bufferGeo.m_vertexBufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        if (bufferSize > 0 && bufferData)
        {
            glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
            GL_ERROR_CHECK();
        }

        // enable vertex attribute array
        bufferGeo.numAttributes = 3;
        bufferGeo.attributeItems = s_attrPos3Uv2Normal;
        for (int i = 0; i < bufferGeo.numAttributes; ++i)
        {
            const AttributeItem& attrItem = bufferGeo.attributeItems[i];

            glEnableVertexAttribArray(i);
            GL_ERROR_CHECK();

            glVertexAttribPointer(i, attrItem.m_size, attrItem.m_glType, GL_FALSE, 32, (GLvoid*)((intptr_t)attrItem.m_offset));
            GL_ERROR_CHECK();
        }

        return true;
    }

    bool uploadIndexBuffer(BufferGeometry& bufferGeo, const uint16 * bufferData, uint32 numIndex)
    {
        // create buffer object
        if (bufferGeo.m_indexBufferId == 0)
        {
            glGenBuffers(1, &bufferGeo.m_indexBufferId);
            GL_ERROR_CHECK();
            if (bufferGeo.m_indexBufferId == 0) return false;
        }

        // Bind the VBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferGeo.m_indexBufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndex * sizeof(uint16), bufferData, GL_STATIC_DRAW);
        GL_ERROR_CHECK();

        bufferGeo.m_numIndex = numIndex;
        return true;
    }

    void GeometryBuilder::buildBox(BufferGeometry& bufferGeo, float width, float height, float depth, int widthSegments, int heightSegments, int depthSegments)
    {
        std::vector<uint16> indis;
        std::vector<VertexPos3Uv2Normal> verts;

        generateBoxData(verts, indis, width, height, depth, widthSegments, heightSegments, depthSegments);

        uploadVertexBuffer(bufferGeo, verts.data(), sizeof(verts[0])*verts.size());
        uploadIndexBuffer(bufferGeo, indis.data(), indis.size());
    }
}
