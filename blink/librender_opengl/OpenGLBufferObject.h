/*!
 * \file OpenGLBufferObject.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <BufferObject.h>
#include <glad/glad.h>

namespace blink
{
    class OpenGLBufferObject : public BufferObject
    {
    public:
        OpenGLBufferObject(BufferType bufferType);
        virtual ~OpenGLBufferObject();

        virtual bool uploadBufferData(const void* data, uint32 size, Usage usage) override;

        static BufferObject* createBuffer(BufferType bufferType);
        static GLenum getGLTarget(BufferType bufferType);
        static GLenum getGLUsage(Usage usage);

    private:
        GLuint m_buffer{};

    };
}
