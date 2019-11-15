/*!
 * \file OpenGLRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include <RenderModule.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

NS_BEGIN

class Shader;

class OpenGLRenderModule : public RenderModule
{
public:
    OpenGLRenderModule();
    virtual ~OpenGLRenderModule();

    BufferObject* createBufferObject(BufferObject::BufferType bufferType);
    bool destroyBufferObject(BufferObject* bufferObject);

    VertexBuffer* createVertexBuffer(BufferAttributes* attributes);
    bool destroyVertexBuffer(VertexBuffer* vertexBuffer);

    Shader* createShaderFromStock(Shader::StockShaders stockShader, uint32 preprocessDefine);
    Shader* createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer);
    bool destroyShader(Shader* shader);

    virtual bool createDevice(const glm::ivec2& deviceSize) override;
    virtual void destroyDevice() override;

    virtual bool gameLoop() override;

private:
    GLFWwindow* m_window{};

};

NS_END
