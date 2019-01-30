#include "OpenGL3RenderSystem.h"
#include "Shader.h"
#include "../camera/CameraData.h"
#include "../scene/TransformData.h"
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
    }

    void OpenGL3RenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // select camera
        if (!m_camera.valid()) return;

        auto cameraData = m_camera.component<CameraData>().get();

        entities.each<Transform, BufferGeometry, PhongMaterial>([&](entityx::Entity entity, Transform& transform, BufferGeometry& geometry, PhongMaterial& material)
        {
            // render the geometry
            glUseProgram(m_shader->getProgramId());
            GL_ERROR_CHECK();

            //material->setupShaderUniforms(m_shader);
            //material->setupShaderSampler(m_shader);

            m_shader->setUniform("u_worldToClip", cameraData->worldToClip);
            m_shader->setUniform("u_localToWorld", transform.localToWorldTransform);
            m_shader->setUniform("u_localToWorldTranInv", glm::transpose(glm::inverse(glm::mat3(transform.localToWorldTransform))));
            m_shader->setUniform("u_localToClip", cameraData->worldToClip * transform.localToWorldTransform);
            m_shader->setUniform("u_viewPos", cameraData->cameraPos);
            m_shader->setUniform("u_diffuseColor", material.diffuseColor);

            // TODO: apply render state

            glBindVertexArray(geometry.vertexArrayObjectId);
            GL_ERROR_CHECK();

            // Bind the IBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.indexBufferId);
            GL_ERROR_CHECK();

            // Draws a indexed triangle array
            glDrawElements(GL_TRIANGLES, geometry.numIndex, GL_UNSIGNED_SHORT, 0);
            GL_ERROR_CHECK();
        });
    }
}
