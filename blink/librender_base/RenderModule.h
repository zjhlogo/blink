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

    typedef BufferObject* (*FuncCreateBufferObject)(BufferObject::BufferType bufferType);
    static FuncCreateBufferObject createBufferObject;

    typedef bool (*FuncDestroyBufferObject)(BufferObject* bufferObject);
    static FuncDestroyBufferObject destroyBufferObject;

    typedef VertexBuffer* (*FuncCreateVertexBuffer)(BufferAttributes* attributes);
    static FuncCreateVertexBuffer createVertexBuffer;

    typedef bool (*FuncDestroyVertexBuffer)(VertexBuffer* vertexBuffer);
    static FuncDestroyVertexBuffer destroyVertexBuffer;

    typedef Shader* (*FuncCreateShaderFromStock)(Shader::StockShaders stockShader, uint32 preprocessDefine);
    static FuncCreateShaderFromStock createShaderFromStock;

    typedef Shader* (*FuncCreateShaderFromBuffer)(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer);
    static FuncCreateShaderFromBuffer createShaderFromBuffer;

    typedef bool (*FuncDestroyShader)(Shader* shader);
    static FuncDestroyShader destroyShader;

    virtual bool createDevice(const glm::ivec2& deviceSize) = 0;
    virtual void destroyDevice() = 0;

    virtual bool gameLoop() = 0;

private:
    tstring m_name;
};

NS_END
