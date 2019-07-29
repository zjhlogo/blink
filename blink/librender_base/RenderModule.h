/*!
 * \file RenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <BaseTypesGlm.h>
#include "BufferObject.h"
#include "VertexBuffer.h"
#include "Shader.h"

namespace blink
{
    class RenderModule
    {
    public:
        RenderModule();
        virtual ~RenderModule();

        typedef BufferObject* (*CreateBufferObjectCb)(BufferObject::BufferType bufferType);
        static CreateBufferObjectCb createBuffer;

        typedef bool (*DestroyBufferObjectCb)(BufferObject* bufferObject);
        static DestroyBufferObjectCb destroyBuffer;

        typedef VertexBuffer* (*CreateVertexBufferCb)();
        static CreateVertexBufferCb createVertexBuffer;

        typedef Shader* (*CreateShaderFromStock)(Shader::StockShaders stockShader, uint32 preprocessDefine);
        static CreateShaderFromStock createShaderFromStock;

        typedef Shader* (*CreateShaderFromBuffer)(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer);
        static CreateShaderFromBuffer createShaderFromBuffer;

        typedef bool(*DestroyShader)(Shader* shader);
        static DestroyShader destroyShader;

        virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
        virtual void destroyDevice() = 0;

        virtual bool gameLoop() = 0;

    };
}
