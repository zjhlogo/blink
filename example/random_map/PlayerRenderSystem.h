#pragma once
#include <entityx/entityx.h>
#include <camera/CameraData.h>
#include "PlayerData.h"
#include "DiffuseMaterial.h"
#include "Atlas.h"
#include <render/geometry/BufferGeometry.h>

class PlayerRenderSystem : public entityx::System<PlayerRenderSystem>, public entityx::Receiver<PlayerRenderSystem>
{
public:
    PlayerRenderSystem();
    virtual ~PlayerRenderSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt);
    void receive(const entityx::ComponentAddedEvent<PlayerData>& evt);

private:
    void addPiece(const Atlas::Piece* piece, const glm::vec3& pos, bool flip = false);
    void render();

private:
    entityx::Entity m_camera;
    entityx::Entity m_player;
    DiffuseMaterial* m_material{};
    Atlas* m_atlas{};

    blink::BufferGeometry* m_geometry{};
    std::vector<blink::VertexPos3Uv2> m_verts;
    std::vector<blink::uint16> m_indis;

    int m_frameIndex{};

    const Atlas::Piece* m_legs[20]{};
    const Atlas::Piece* m_body[20]{};
    const Atlas::Piece* m_head[20]{};
    const Atlas::Piece* m_arms[20]{};

};
