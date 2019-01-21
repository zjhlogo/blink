#include "OpenGL3RenderSystem.h"
#include "../Components.h"
#include "../Materials.h"
#include "../Shader.h"
#include "GlConfig.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    OpenGL3RenderSystem::~OpenGL3RenderSystem()
    {
        SAFE_RELEASE(m_shader);
    }

    void OpenGL3RenderSystem::configure(entityx::EventManager & events)
    {
        // depth test setup
        glEnable(GL_DEPTH_TEST);

        // cull face setup
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        // blend mode setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        m_shader = Shader::fromStock(Shader::StockShaders::Wireframe, 0);
        m_cameraPos = { 0.0f, 0.0f, 3.0f };
        m_worldToCamera = glm::lookAt(m_cameraPos, VEC3_ZERO, VEC3_PY);
        m_cameraToClip = glm::perspectiveFov(45.0f, 1280.0f, 720.0f, 0.1f, 100.0f);
        m_worldToClip = m_cameraToClip * m_worldToCamera;
    }

    void OpenGL3RenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        entities.each<Transform, BufferGeometry, PhongMaterial>([&](entityx::Entity entity, Transform& transform, BufferGeometry& geometry, PhongMaterial& material)
        {
            // TODO: render the geometry
            glUseProgram(m_shader->getProgramId());
            GL_ERROR_CHECK();

            //camera->setupShaderUniforms(transform.localToWorldTransform, m_shader);
            //material->setupShaderUniforms(m_shader);
            //material->setupShaderSampler(m_shader);

            m_shader->setUniform("u_worldToClip", m_worldToClip);
            m_shader->setUniform("u_localToWorld", transform.localToWorldTransform);
            m_shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(transform.localToWorldTransform))));
            m_shader->setUniform("u_localToClip", m_worldToClip * transform.localToWorldTransform);
            m_shader->setUniform("u_viewPos", m_cameraPos);
            m_shader->setUniform("u_diffuseColor", material.diffuseColor);

            // TODO: apply render state

            glBindVertexArray(geometry.m_vertexArrayObjectId);
            GL_ERROR_CHECK();

            // Bind the IBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.m_indexBufferId);
            GL_ERROR_CHECK();

            // Draws a indexed triangle array
            glDrawElements(GL_TRIANGLES, geometry.m_numIndex, GL_UNSIGNED_SHORT, 0);
            GL_ERROR_CHECK();
        });
    }
}
