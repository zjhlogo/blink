#include "OpenGL3RenderSystem.h"
#include "shader/Shader.h"
#include "../scene/TransformData.h"
#include "GlConfig.h"
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

namespace blink
{
    OpenGL3RenderSystem::~OpenGL3RenderSystem()
    {

    }

    void OpenGL3RenderSystem::configure(entityx::EventManager & events)
    {
        RenderSystem::configure(events);

        // depth test setup
        glEnable(GL_DEPTH_TEST);

        // cull face setup
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        // blend mode setup
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGL3RenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {
        glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        std::vector<entityx::Entity> lightEntities;
        // collect lights
        entities.each<TransformData, LightData>([&](entityx::Entity entity, TransformData& transform, LightData& light)
        {
            lightEntities.push_back(entity);
        });

        // select camera
        if (!m_camera.valid()) return;
        auto cameraData = m_camera.component<CameraData>().get();

        entities.each<TransformData, MeshData>([&](entityx::Entity entity, TransformData& transform, MeshData& mesh)
        {
            Shader* shader = mesh.material->getShader();
            if (!shader) return;

            // TODO: apply render state

            // render the geometry
            glUseProgram(shader->getProgramId());
            GL_ERROR_CHECK();

            // setup shader uniforms for material
            mesh.material->setupShaderUniforms(shader);
            mesh.material->setupShaderSampler(shader);

            // setup shader uniforms for camera
            cameraData->setupShaderUniforms(transform.localToWorldTransform, shader);
            
            // setup shader uniforms for lights
            for (auto& lightEntity : lightEntities)
            {
                lightEntity.component<LightData>()->light->setupShaderUniforms(shader, *lightEntity.component<TransformData>().get());
            }

            glBindVertexArray(mesh.geometry->getVertexArrayObjectId());
            GL_ERROR_CHECK();

            // Bind the IBO
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.geometry->getIndexBufferId());
            GL_ERROR_CHECK();

            // Draws a indexed triangle array
            glDrawElements(GL_TRIANGLES, mesh.geometry->getNumIndex(), GL_UNSIGNED_SHORT, 0);
            GL_ERROR_CHECK();
        });
    }
}
