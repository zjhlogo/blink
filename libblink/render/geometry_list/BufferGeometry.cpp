#include "BufferGeometry.h"
#include <glad/glad.h>

namespace blink
{
    BufferGeometry::BufferGeometry()
    {

    }

    BufferGeometry::~BufferGeometry()
    {

    }

    bool BufferGeometry::uploadVertexBuffer(const BufferAttributes * bufferAttribute, const void * bufferData, uint32 bufferSize)
    {
        // create buffer object
        if (m_vertexBufferId == 0)
        {
            glGenBuffers(1, &m_bufferId);
            GL_ERROR_CHECK();
            if (m_bufferId == 0) return false;
        }

        // Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        glBufferData(GL_ARRAY_BUFFER, m_bufferData.size(), m_bufferData.data(), GL_STATIC_DRAW);
        GL_ERROR_CHECK();

        // Unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return true;
    }

    bool BufferGeometry::uploadIndexBuffer(const uint16 * bufferData, uint32 numIndex)
    {
        return false;
    }
}
