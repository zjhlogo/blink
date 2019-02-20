#include "PlayerRenderSystem.h"
#include "ArmorAtlas.h"
#include <glad/glad.h>
#include <render/GlConfig.h>
#include <render/shader/Shader.h>
#include <render/texture/Texture2D.h>

PlayerRenderSystem::PlayerRenderSystem()
{
}

PlayerRenderSystem::~PlayerRenderSystem()
{
    SAFE_DELETE(m_geometry);
    SAFE_DELETE(m_material);
    SAFE_DELETE(m_atlas);
}

void PlayerRenderSystem::configure(entityx::EventManager & events)
{
    events.subscribe<entityx::ComponentAddedEvent<blink::CameraData>>(*this);
    events.subscribe<entityx::ComponentAddedEvent<PlayerData>>(*this);

    m_atlas = new ArmorAtlas();
    m_atlas->initialize();

    m_material = new DiffuseMaterial();
    m_material->setTexture("tex_diffuse", m_atlas->getTexture(), 0);

    m_geometry = new blink::BufferGeometry();
}

void PlayerRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // render player
    if (!m_player.valid()) return;

    auto playerData = m_player.component<PlayerData>().get();

    // add leg render data
    addPiece("Armor_Legs_1_00_00", playerData->position);

    // add body render data
    addPiece("Armor_Body_1_00_00", playerData->position);

    // add head render data
    addPiece("Armor_Head_1_00_00", playerData->position);

    // add arm render data
    addPiece("Armor_Arm_1_00_00", playerData->position);

    if (!m_verts.empty())
    {
        m_geometry->uploadVertexBuffer(blink::BufferAttributes::StockAttributes::Pos3Uv2, m_verts.data(), sizeof(m_verts[0])* m_verts.size());
        m_geometry->uploadIndexBuffer(m_indis.data(), m_indis.size());
        m_verts.clear();
        m_indis.clear();

        render();
    }
}

void PlayerRenderSystem::receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt)
{
    m_camera = evt.entity;
}

void PlayerRenderSystem::receive(const entityx::ComponentAddedEvent<PlayerData>& evt)
{
    m_player = evt.entity;
}

void PlayerRenderSystem::addPiece(const blink::tstring & name, const glm::vec3 & pos)
{
    auto piece = m_atlas->findPiece(name);
    if (!piece) return;

    float offsetX = pos.x + piece->offset.x;
    float offsetY = pos.y + piece->offset.y;

    int numVerts = static_cast<int>(m_verts.size());
    m_verts.push_back({ offsetX,                     offsetY,                     1.0f, piece->uvs[0].s, piece->uvs[0].t });
    m_verts.push_back({ offsetX + piece->trimSize.x, offsetY,                     1.0f, piece->uvs[1].s, piece->uvs[1].t });
    m_verts.push_back({ offsetX,                     offsetY + piece->trimSize.y, 1.0f, piece->uvs[2].s, piece->uvs[2].t });
    m_verts.push_back({ offsetX + piece->trimSize.x, offsetY + piece->trimSize.y, 1.0f, piece->uvs[3].s, piece->uvs[3].t });

    m_indis.push_back(numVerts + 0);
    m_indis.push_back(numVerts + 2);
    m_indis.push_back(numVerts + 1);
    m_indis.push_back(numVerts + 1);
    m_indis.push_back(numVerts + 2);
    m_indis.push_back(numVerts + 3);
}

void PlayerRenderSystem::render()
{
    // select camera
    if (!m_camera.valid()) return;
    auto cameraData = m_camera.component<blink::CameraData>().get();

    auto shader = m_material->getShader();
    if (!shader) return;

    // render the geometry
    glUseProgram(shader->getProgramId());
    GL_ERROR_CHECK();

    // setup shader uniforms for material
    m_material->setupShaderUniforms(shader);
    m_material->setupShaderSampler(shader);

    // setup shader uniforms for camera
    {
        shader->setUniform("u_worldToClip", cameraData->worldToClip);
        shader->setUniform("u_localToWorld", blink::MAT4_IDENTITY);
        shader->setUniform("u_localToWorldTranInv", blink::MAT3_IDENTITY);
        shader->setUniform("u_localToClip", cameraData->worldToClip);
        shader->setUniform("u_viewPos", cameraData->cameraPos);
    }

    glBindVertexArray(m_geometry->getVertexArrayObjectId());
    GL_ERROR_CHECK();

    // Bind the IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_geometry->getIndexBufferId());
    GL_ERROR_CHECK();

    // Draws a indexed triangle array
    glDrawElements(GL_TRIANGLES, m_geometry->getNumIndex(), GL_UNSIGNED_SHORT, 0);
    GL_ERROR_CHECK();
}
