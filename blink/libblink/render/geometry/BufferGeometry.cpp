#include "BufferGeometry.h"
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
        destroyVertexArrayObject();
    }

    bool BufferGeometry::uploadVertexBuffer(BufferAttributes::StockAttributes stockAttr, const void * bufferData, uint32 bufferSize, uint32 usage)
    {
        auto bufferAttributes = blink::BufferAttributes::fromStock(stockAttr);
        return uploadVertexBuffer(bufferAttributes, bufferData, bufferSize, usage);
    }

    bool BufferGeometry::uploadVertexBuffer(std::shared_ptr<BufferAttributes> bufferAttribute, const void * bufferData, uint32 bufferSize, uint32 usage)
    {
        if (!bufferAttribute) return false;

        // create buffer object
        if (m_vertexBufferId == 0)
        {
            glGenBuffers(1, &m_vertexBufferId);
            GL_ERROR_CHECK();
            if (m_vertexBufferId == 0) return false;
        }

        // create vao
        if (m_vertexArrayObjectId == 0)
        {
            glGenVertexArrays(1, &m_vertexArrayObjectId);
            GL_ERROR_CHECK();
            if (m_vertexArrayObjectId == 0) return false;
        }

        // bind vao
        glBindVertexArray(m_vertexArrayObjectId);
        GL_ERROR_CHECK();

        // Bind the VBO
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        GL_ERROR_CHECK();

        // Set the buffer's data
        if (bufferSize > 0 && bufferData)
        {
            glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, usage);
            GL_ERROR_CHECK();
        }

        // enable vertex attribute array
        m_vertexBufferAttribute = bufferAttribute;
        for (int i = 0; i < m_vertexBufferAttribute->getNumAttributeItems(); ++i)
        {
            const blink::BufferAttributes::AttributeItem* pAttrItem = m_vertexBufferAttribute->getAttributeItem(i);

            glEnableVertexAttribArray(i);
            GL_ERROR_CHECK();

            glVertexAttribPointer(i, pAttrItem->m_size, pAttrItem->m_glType, GL_FALSE, m_vertexBufferAttribute->getStride(), (GLvoid*)((intptr_t)pAttrItem->m_offset));
            GL_ERROR_CHECK();
        }

        return true;
    }

    bool BufferGeometry::uploadIndexBuffer(const uint16 * bufferData, uint32 numIndex, uint32 usage)
    {
		// create buffer object
        if (m_indexBufferId == 0)
        {
            glGenBuffers(1, &m_indexBufferId);
            GL_ERROR_CHECK();
            if (m_indexBufferId == 0) return false;
        }

		// Bind the VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
		GL_ERROR_CHECK();

		// Set the buffer's data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndex*sizeof(uint16), bufferData, usage);
		GL_ERROR_CHECK();

        m_numIndex = numIndex;
		return true;
	}

    void BufferGeometry::destroyVertexArrayObject()
    {
        if (m_vertexArrayObjectId != 0)
        {
            glDeleteVertexArrays(1, &m_vertexArrayObjectId);
            m_vertexArrayObjectId = 0;
        }
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
