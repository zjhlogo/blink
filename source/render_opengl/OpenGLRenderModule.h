/*!
 * \file OpenGLRenderModule.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <render_base/RenderModule.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

NS_BEGIN

class Shader;

class OpenGLRenderModule : public RenderModule
{
public:
    OpenGLRenderModule();
    virtual ~OpenGLRenderModule();

    virtual bool createDevice(const glm::ivec2& deviceSize) override;
    virtual void destroyDevice() override;

    virtual BufferObject* createBufferObject(BufferObject::BufferType bufferType) override;
    virtual bool destroyBufferObject(BufferObject* bufferObject) override;

    virtual VertexBuffer* createVertexBuffer(BufferAttributes* attributes) override;
    virtual bool destroyVertexBuffer(VertexBuffer* vertexBuffer) override;

    virtual Shader* createShaderFromStock(Shader::StockShaders stockShader, uint32 preprocessDefine) override;
    virtual Shader* createShaderFromBuffer(const char* vsBuffer, const char* gsBuffer, const char* fsBuffer) override;
    virtual bool destroyShader(Shader* shader) override;

    virtual Texture* createTexture2D(const tstring& texFile) override;
    virtual Texture* createDepthTexture(int width, int height) override;
    virtual bool destroyTexture(Texture*& texture) override;

    virtual bool gameLoop() override;

private:
    GLFWwindow* m_window{};
};

NS_END
