/*!
 * \file OpenGLBufferObject.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "OpenGLBufferObject.h"
#include "GlConfig.h"
#include <cassert>

NS_BEGIN

OpenGLBufferObject::OpenGLBufferObject(BufferType bufferType)
    :BufferObject(bufferType)
{
    // create buffer object
    glGenBuffers(1, &m_buffer);
    GL_ERROR_CHECK();

    assert(m_buffer != 0);
}

OpenGLBufferObject::~OpenGLBufferObject()
{

}

bool OpenGLBufferObject::uploadBufferData(const void* data, uint32 size, Usage usage)
{
    assert(m_buffer != 0);

    // Bind the VBO
    glBindBuffer(getGLTarget(m_bufferType), m_buffer);
    GL_ERROR_CHECK();

    // Set the buffer's data
    if (size > 0 && data)
    {
        glBufferData(getGLTarget(m_bufferType), size, data, getGLUsage(usage));
        GL_ERROR_CHECK();

        return true;
    }

    return false;
}

BufferObject* OpenGLBufferObject::createBuffer(BufferType bufferType)
{
    OpenGLBufferObject* bufferObj = new OpenGLBufferObject(bufferType);
    // TODO: some other prepare
    return bufferObj;
}

GLenum OpenGLBufferObject::getGLTarget(BufferType bufferType)
{
    static const GLenum GL_TARGET_MAP[static_cast<int>(BufferType::NumBufferType)] =
    {
        GL_ARRAY_BUFFER,			// ArrayBuffer
        GL_ELEMENT_ARRAY_BUFFER,	// ElementArrayBuffer
    };

    if (bufferType < BufferType::ArrayBuffer || bufferType >= BufferType::NumBufferType) return GL_ARRAY_BUFFER;
    return GL_TARGET_MAP[static_cast<int>(bufferType)];
}

GLenum OpenGLBufferObject::getGLUsage(Usage usage)
{
    static const GLenum GL_USAGE_MAP[static_cast<int>(Usage::NumUsage)] =
    {
        GL_STREAM_DRAW,
        GL_STREAM_READ,
        GL_STREAM_COPY,
        GL_STATIC_DRAW,
        GL_STATIC_READ,
        GL_STATIC_COPY,
        GL_DYNAMIC_DRAW,
        GL_DYNAMIC_READ,
        GL_DYNAMIC_COPY,
    };

    if (usage < Usage::StreamDraw || usage >= Usage::NumUsage) return GL_STATIC_DRAW;
    return GL_USAGE_MAP[static_cast<int>(usage)];
}

NS_END
