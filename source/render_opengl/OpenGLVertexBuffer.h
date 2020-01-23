/*!
 * \file OpenGLVertexBuffer.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <render_base/VertexBuffer.h>
#include <glad/glad.h>

NS_BEGIN

class OpenGLRenderModule;

class OpenGLVertexBuffer : public VertexBuffer
{
    friend OpenGLRenderModule;

public:
    OpenGLVertexBuffer(const BufferAttributes* attributes);
    virtual ~OpenGLVertexBuffer();

    virtual bool uploadBuffer(const void* buffer, uint32 size, BufferObject::Usage usage) override;

    static VertexBuffer* createVertexBuffer(const BufferAttributes* attributes);
    static GLenum getGLType(BufferAttributes::AttributeItemType type);

private:
    GLuint m_vertexArray{};
    BufferObject* m_vertexBuffer{};

};

NS_END
