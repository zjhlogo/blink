/*!
 * \file OpenGLVertexBufferObject.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <VertexBuffer.h>
#include <glad/glad.h>

namespace blink
{
    class OpenGLVertexBufferObject : public VertexBuffer
    {
    public:
        OpenGLVertexBufferObject(const BufferAttributes* attributes);
        virtual ~OpenGLVertexBufferObject();

        virtual bool uploadBuffer(const void* buffer, uint32 size, BufferObject::Usage usage) override;

        static VertexBuffer* createVertexBuffer(const BufferAttributes* attributes);
        static GLenum getGLType(BufferAttributes::AttributeItemType type);

    private:
        GLuint m_vertexArray{};
        BufferObject* m_vertexBuffer{};

    };
}
