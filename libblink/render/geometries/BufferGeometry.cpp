#include "BufferGeometry.h"
#include "BufferAttributes.h"
#include "../GlConfig.h"
#include <glad/glad.h>

namespace blink
{
    BufferGeometry::BufferGeometry()
    {

    }

    BufferGeometry::~BufferGeometry()
    {
        destroyVertexBuffer();
        destroyIndexBuffer();
    }

    bool BufferGeometry::uploadVertexBuffer(const BufferAttributes * bufferAttribute, const void * bufferData, uint32 bufferSize)
    {
        // create buffer object
        destroyVertexBuffer();
        glGenBuffers(1, &m_vertexBufferId);
        GL_ERROR_CHECK();
        if (m_vertexBufferId == 0) return false;

        // Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_STATIC_DRAW);
        GL_ERROR_CHECK();

        // Unbind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        return true;
    }

    bool BufferGeometry::uploadIndexBuffer(const uint16 * bufferData, uint32 numIndex)
    {
		// create buffer object
        destroyIndexBuffer();
        glGenBuffers(1, &m_indexBufferId);
        GL_ERROR_CHECK();
        if (m_indexBufferId == 0) return false;

		// Bind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		GL_ERROR_CHECK();

		// Set the buffer's data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndex*sizeof(uint16), bufferData, GL_STATIC_DRAW);
		GL_ERROR_CHECK();

		// Unbind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		return true;
	}
    void BufferGeometry::destroyVertexBuffer()
    {
        if (m_vertexBufferId != 0)
        {
            glDeleteBuffers(1, &m_vertexBufferId);
            m_vertexBufferId = 0;
        }
    }

    void BufferGeometry::destroyIndexBuffer()
    {
        if (m_indexBufferId != 0)
        {
            glDeleteBuffers(1, &m_indexBufferId);
            m_indexBufferId = 0;
        }
    }
}
