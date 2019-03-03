#pragma once
#include <entityx/entityx.h>
#include <input/InputData.h>
#include "PlayerData.h"

class PlayerControlSystem : public entityx::System<PlayerControlSystem>, public entityx::Receiver<PlayerControlSystem>
{
public:
    enum KEY_CODE
    {
        KEY_UP,
        KEY_DOWN,
        KEY_LEFT,
        KEY_RIGHT,
        NUM_KEYS,
    };

public:
    PlayerControlSystem();
    virtual ~PlayerControlSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const blink::KeyboardEvent& evt);
    void receive(const entityx::ComponentAddedEvent<PlayerData>& evt);

private:
    int getMovingFrameIndex(int frameIndex, entityx::TimeDelta dt);

private:
    bool m_keys[NUM_KEYS]{};
    entityx::Entity m_player;
    float m_speed{ 60.0f };

    float m_elapseTime{};
    float m_frameTime{ 0.1f };

};
