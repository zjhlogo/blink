#include "MapRenderSystem.h"
#include <glad/glad.h>
#include <render/GlConfig.h>
#include <render/shader/Shader.h>

MapRenderSystem::MapRenderSystem()
{
}

MapRenderSystem::~MapRenderSystem()
{
    SAFE_RELEASE(m_material);
    SAFE_DELETE(m_renderBlock);
}

void MapRenderSystem::configure(entityx::EventManager & events)
{
    events.subscribe<MapDataUpdateEvent>(*this);
    events.subscribe<entityx::ComponentAddedEvent<blink::CameraData>>(*this);

    // depth test setup
    glEnable(GL_DEPTH_TEST);

    // cull face setup
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

    // blend mode setup
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_material = new MapTileMaterial();
}

void MapRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    glClearColor(0.1f, 0.3f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // select camera
    if (!m_camera.valid()) return;
    auto cameraData = m_camera.component<blink::CameraData>().get();

    blink::Shader* shader = m_material->getShader();
    if (!shader) return;

    // render the geometry
    glUseProgram(shader->getProgramId());
    GL_ERROR_CHECK();

    // setup shader uniforms for material
    m_material->setupShaderUniforms(shader);
    m_material->setupShaderSampler(shader);

    // setup shader uniforms for camera
    cameraData->setupShaderUniforms(blink::MAT4_IDENTITY, shader);

    glBindVertexArray(m_renderBlock->getVertexArrayObjectId());
    GL_ERROR_CHECK();

    // Bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderBlock->getIndexBufferId());
    GL_ERROR_CHECK();

    // Draws a indexed triangle array
    glDrawElements(GL_TRIANGLES, m_renderBlock->getNumIndex(), GL_UNSIGNED_SHORT, 0);
    GL_ERROR_CHECK();
}

void MapRenderSystem::receive(const MapDataUpdateEvent & evt)
{
    // rebuild map render block
    SAFE_DELETE(m_renderBlock);
    m_renderBlock = new MapRenderBlock(evt.mapData, glm::ivec2(0, 0), glm::ivec2(evt.mapData->width, evt.mapData->height));
    m_renderBlock->generateGeometry();
}

void MapRenderSystem::receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt)
{
    m_camera = evt.entity;
}
