#include "HelloWorldApp.h"
#include <Framework.h>
#include <render/GlConfig.h>
#include <render/geometries/BoxGeometry.h>
#include <render/RenderModule.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv)
{
    if (!blink::Framework::getInstance().initialize(new HelloWorldApp()))
    {
        blink::Framework::getInstance().terminate();
        return -1;
    }

	int result = blink::Framework::getInstance().start();
    blink::Framework::getInstance().terminate();

    return result;
}

bool HelloWorldApp::initialize()
{
    blink::Framework::getInstance().insertComponent(new blink::RenderModule());

    m_worldToCamera = glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_cameraToClip = glm::perspectiveFov(45.0f, 1280.0f, 720.0f, 1.0f, 1000.0f);

    m_boxGeometry = new blink::BoxGeometry(1.0f, 1.0f, 1.0f);
    m_shader = blink::Shader::fromStock(blink::Shader::StockShaders::Lamber);

    m_matModel = blink::MAT4_IDENTITY;

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_RELEASE(m_shader);
    SAFE_DELETE(m_boxGeometry);
}

void HelloWorldApp::update(float dt)
{
    m_matModel = glm::rotate(m_matModel, 0.5f * dt, glm::vec3(1.0f, 0.5f, 0.7f));
}

void HelloWorldApp::render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader->getProgramId());
    GL_ERROR_CHECK();

    m_shader->setUniform("u_localToClip", m_cameraToClip * m_worldToCamera * m_matModel);
    m_shader->setUniform("u_lightDir", blink::VEC3_NZ);

    // TODO: apply render state

    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_boxGeometry->getVertexBufferId());
    GL_ERROR_CHECK();

    const blink::BufferAttributes* pVertAttributes = m_boxGeometry->getBufferAttributes();
    int numAttrs = pVertAttributes->getNumAttributeItems();
    int stride = pVertAttributes->getStride();

    // enable vertex attribute array
    for (int i = 0; i < numAttrs; ++i)
    {
        const blink::BufferAttributes::AttributeItem* pAttrItem = pVertAttributes->getAttributeItem(i);

        glEnableVertexAttribArray(i);
        GL_ERROR_CHECK();

        glVertexAttribPointer(i, pAttrItem->m_size, pAttrItem->m_glType, GL_FALSE, stride, (GLvoid*)((intptr_t)pAttrItem->m_offset));
        GL_ERROR_CHECK();
    }

    // Bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_boxGeometry->getIndexBufferId());
    GL_ERROR_CHECK();

    // Draws a indexed triangle array
    glDrawElements(GL_TRIANGLES, m_boxGeometry->getNumIndex(), GL_UNSIGNED_SHORT, 0);
    GL_ERROR_CHECK();

    // Unbind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Unbind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
