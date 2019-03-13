#pragma once
#include <entityx/entityx.h>
#include <input/InputData.h>
#include "PlayerData.h"
#include "MapData.h"

class PlayerControlSystem : public entityx::System<PlayerControlSystem>, public entityx::Receiver<PlayerControlSystem>
{
public:
    enum MOVEMENT
    {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        NUM_MOVES,
    };

public:
    PlayerControlSystem();
    virtual ~PlayerControlSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const blink::KeyboardEvent& evt);
    void receive(const entityx::ComponentAddedEvent<PlayerData>& evt);
    void receive(const entityx::ComponentAddedEvent<MapData>& evt);

private:
    int getMovingFrameIndex(int frameIndex, entityx::TimeDelta dt);
    glm::vec3 calculatePosition(const MapData* mapData, const glm::vec3& oldPos, float speed, float dt);

private:
    bool m_keys[NUM_MOVES]{};
    entityx::Entity m_player;
    entityx::Entity m_mapData;
    float m_speed{ 60.0f };

    float m_elapseTime{};
    float m_frameTime{ 0.1f };

};
