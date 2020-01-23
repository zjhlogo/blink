/*!
 * \file RenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include "BufferObject.h"
#include "Shader.h"
#include "VertexBuffer.h"

#include <BaseTypesGlm.h>

NS_BEGIN

class RenderModule
{
public:
    RenderModule(const tstring& name);
    virtual ~RenderModule();

    const tstring& getName();

    static RenderModule& getInstance();

    virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
    virtual void destroyDevice() = 0;

    virtual BufferObject* createBufferObject(BufferObject::BufferType bufferType) = 0;
    virtual bool destroyBufferObject(BufferObject* bufferObject) = 0;

    virtual VertexBuffer* createVertexBuffer(BufferAttributes* attributes) = 0;
    virtual bool destroyVertexBuffer(VertexBuffer* vertexBuffer) = 0;

    virtual Shader* createShaderFromStock(Shader::StockShaders stockShader, uint32 preprocessDefine) = 0;
    virtual Shader* createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer) = 0;
    virtual bool destroyShader(Shader* shader) = 0;

    virtual Texture* createTexture2D(const tstring& texFile) = 0;
    virtual Texture* createDepthTexture(int width, int height) = 0;
    virtual bool destroyTexture(Texture*& texture) = 0;

    virtual bool gameLoop() = 0;

private:
    tstring m_name;

    static RenderModule* s_renderModule;
};

NS_END
