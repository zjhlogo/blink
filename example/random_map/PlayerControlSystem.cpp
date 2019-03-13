#include "PlayerControlSystem.h"
#include "MapUtilities.h"
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
    events.subscribe<entityx::ComponentAddedEvent<MapData>>(*this);
}

void PlayerControlSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    // render player
    if (!m_player.valid()) return;

    auto playerData = m_player.component<PlayerData>().get();
    auto mapData = m_mapData.component<MapData>().get();

    if (m_keys[MOVE_RIGHT])
    {
        playerData->position = calculatePosition(mapData, playerData->position, m_speed, dt);
        playerData->facingRight = true;
        playerData->status = PlayerData::STATUS_MOVING;
        playerData->frameIndex = getMovingFrameIndex(playerData->frameIndex, dt);
    }
    else if (m_keys[MOVE_LEFT])
    {
        playerData->position = calculatePosition(mapData, playerData->position, -m_speed, dt);
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
    MOVEMENT index = MOVE_UP;
    switch (evt.key)
    {
    case GLFW_KEY_W:
        index = MOVE_UP;
        break;
    case GLFW_KEY_A:
        index = MOVE_LEFT;
        break;
    case GLFW_KEY_S:
        index = MOVE_DOWN;
        break;
    case GLFW_KEY_D:
        index = MOVE_RIGHT;
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

void PlayerControlSystem::receive(const entityx::ComponentAddedEvent<MapData>& evt)
{
    m_mapData = evt.entity;
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

glm::vec3 PlayerControlSystem::calculatePosition(const MapData* mapData, const glm::vec3 & oldPos, float speed, float dt)
{
    glm::ivec2 oldGridPos = MapUtilities::getGridPos(mapData, oldPos);

    // calculate newPos
    glm::vec3 newPos = oldPos;
    newPos.x += speed * dt;

    // get grid pos for newPos
    glm::ivec2 newGridPos = MapUtilities::getGridPos(mapData, newPos);
    glm::vec3 newWorldPos = MapUtilities::getWorldPos(mapData, newGridPos);

    if (oldGridPos == newGridPos) return newPos;

    // crossable
    if (MapUtilities::crossable(mapData, oldGridPos, newGridPos))
    {
        glm::ivec2 groundGridPos = MapUtilities::findGroundGridPos(mapData, newGridPos);
        newWorldPos = MapUtilities::getWorldPos(mapData, groundGridPos);
        newPos.y = newWorldPos.y;
        return newPos;
    }

    // not crossable
    newPos.x = newWorldPos.x;
    return newPos;
}
