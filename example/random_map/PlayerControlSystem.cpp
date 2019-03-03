#include "PlayerControlSystem.h"
#include <GLFW/glfw3.h>

PlayerControlSystem::PlayerControlSystem()
{
}

PlayerControlSystem::~PlayerControlSystem()
{
}

void PlayerControlSystem::configure(entityx::EventManager & events)
{
    events.subscribe<blink::KeyboardEvent>(*this);
    events.subscribe<entityx::ComponentAddedEvent<PlayerData>>(*this);
}

void PlayerControlSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // render player
    if (!m_player.valid()) return;

    auto playerData = m_player.component<PlayerData>().get();

    if (m_keys[KEY_RIGHT])
    {
        playerData->position.x += (dt * m_speed);
        playerData->facingRight = true;
        playerData->status = PlayerData::STATUS_MOVING;
        playerData->frameIndex = getMovingFrameIndex(playerData->frameIndex, dt);
    }
    else if (m_keys[KEY_LEFT])
    {
        playerData->position.x -= (dt * m_speed);
        playerData->facingRight = false;
        playerData->status = PlayerData::STATUS_MOVING;
        playerData->frameIndex = getMovingFrameIndex(playerData->frameIndex, dt);
    }
    else
    {
        playerData->status = PlayerData::STATUS_STANDBY;
        playerData->frameIndex = 0;
    }
}

void PlayerControlSystem::receive(const blink::KeyboardEvent & evt)
{
    KEY_CODE index = KEY_UP;
    switch (evt.key)
    {
    case GLFW_KEY_W:
        index = KEY_UP;
        break;
    case GLFW_KEY_A:
        index = KEY_LEFT;
        break;
    case GLFW_KEY_S:
        index = KEY_DOWN;
        break;
    case GLFW_KEY_D:
        index = KEY_RIGHT;
        break;
    default:
        return;
        break;
    }

    m_keys[index] = (evt.action != GLFW_RELEASE);
}

void PlayerControlSystem::receive(const entityx::ComponentAddedEvent<PlayerData>& evt)
{
    m_player = evt.entity;
}

int PlayerControlSystem::getMovingFrameIndex(int frameIndex, entityx::TimeDelta dt)
{
    if (frameIndex < 6 || frameIndex > 19)
    {
        m_elapseTime = dt;
        return 6;
    }

    m_elapseTime += dt;
    if (m_elapseTime > m_frameTime)
    {
        ++frameIndex;
        m_elapseTime -= m_frameTime;
    }

    if (frameIndex > 19) frameIndex = 6;

    return frameIndex;
}
