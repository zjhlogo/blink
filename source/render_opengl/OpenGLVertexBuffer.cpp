/*!
 * \file OpenGLVertexBuffer.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "OpenGLVertexBuffer.h"
#include "GlConfig.h"

#include <render_base/RenderModule.h>

NS_BEGIN

OpenGLVertexBuffer::OpenGLVertexBuffer(const BufferAttributes* attributes)
    : VertexBuffer(attributes)
{
    // create vao
    glGenVertexArrays(1, &m_vertexArray);
    GL_ERROR_CHECK();
    assert(m_vertexArray != 0);

    // bind vao
    glBindVertexArray(m_vertexArray);
    GL_ERROR_CHECK();

    m_vertexBuffer = RenderModule::getInstance().createBufferObject(BufferObject::BufferType::ArrayBuffer);
    assert(m_vertexBuffer);

    // enable vertex attribute array
    for (int i = 0; i < attributes->getNumAttributeItems(); ++i)
    {
        const NS::BufferAttributes::AttributeItem* pAttrItem = attributes->getAttributeItem(i);

        glEnableVertexAttribArray(i);
        GL_ERROR_CHECK();

        glVertexAttribPointer(i, pAttrItem->size, getGLType(pAttrItem->type), GL_FALSE, attributes->getStride(), (GLvoid*)((intptr_t)pAttrItem->offset));
        GL_ERROR_CHECK();
    }

    // TODO: unbind vao
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    RenderModule::getInstance().destroyBufferObject(m_vertexBuffer);
    m_vertexBuffer = nullptr;

    glDeleteVertexArrays(1, &m_vertexArray);
    m_vertexArray = 0;
}

bool OpenGLVertexBuffer::uploadBuffer(const void* buffer, uint32 size, BufferObject::Usage usage)
{
    assert(m_vertexBuffer);
    return m_vertexBuffer->uploadBufferData(buffer, size, usage);
}

VertexBuffer* OpenGLVertexBuffer::createVertexBuffer(const BufferAttributes* attributes)
{
    auto vertexBuffer = new OpenGLVertexBuffer(attributes);
    return vertexBuffer;
}

GLenum OpenGLVertexBuffer::getGLType(BufferAttributes::AttributeItemType type)
{
    static const GLenum GL_TYPE_MAP[static_cast<int>(BufferAttributes::AttributeItemType::NumberOfAttributeItemType)] = {
        GL_FLOAT,          // AT_UNKNOWN,
        GL_BYTE,           // AT_BYTE,
        GL_UNSIGNED_BYTE,  // AT_UNSIGNED_BYTE,
        GL_SHORT,          // AT_SHORT,
        GL_UNSIGNED_SHORT, // AT_UNSIGNED_SHORT,
        GL_FLOAT,          // AT_FLOAT,
    };

    if (type < BufferAttributes::AttributeItemType::Unknown || type >= BufferAttributes::AttributeItemType::NumberOfAttributeItemType) return GL_FLOAT;
    return GL_TYPE_MAP[static_cast<int>(type)];
}

NS_END
