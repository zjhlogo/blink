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

NS_BEGIN

class RenderModule
{
public:
    RenderModule();
    virtual ~RenderModule();

    typedef BufferObject* (*CreateBufferObjectCb)(BufferObject::BufferType bufferType);
    static CreateBufferObjectCb createBufferObject;

    typedef bool(*DestroyBufferObjectCb)(BufferObject* bufferObject);
    static DestroyBufferObjectCb destroyBufferObject;

    typedef VertexBuffer* (*CreateVertexBufferCb)(BufferAttributes* attributes);
    static CreateVertexBufferCb createVertexBuffer;

    typedef bool(*DestroyVertexBufferCb)(VertexBuffer* vertexBuffer);
    static DestroyVertexBufferCb destroyVertexBuffer;

    typedef Shader* (*CreateShaderFromStockCb)(Shader::StockShaders stockShader, uint32 preprocessDefine);
    static CreateShaderFromStockCb createShaderFromStock;

    typedef Shader* (*CreateShaderFromBufferCb)(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer);
    static CreateShaderFromBufferCb createShaderFromBuffer;

    typedef bool(*DestroyShaderCb)(Shader* shader);
    static DestroyShaderCb destroyShader;

    virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
    virtual void destroyDevice() = 0;

    virtual bool gameLoop() = 0;

};

NS_END
