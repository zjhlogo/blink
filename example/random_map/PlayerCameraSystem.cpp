#include "PlayerCameraSystem.h"
#include <glm/gtc/matrix_transform.hpp>

PlayerCameraSystem::~PlayerCameraSystem()
{
}

void PlayerCameraSystem::configure(entityx::EventManager & events)
{
    events.subscribe<entityx::ComponentAddedEvent<PlayerData>>(*this);
    events.subscribe<entityx::ComponentAddedEvent<NS::CameraData>>(*this);
}

void PlayerCameraSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    if (!m_player.valid()) return;
    if (!m_camera.valid()) return;

    auto playerData = m_player.component<PlayerData>().get();
    auto cameraData = m_camera.component<NS::CameraData>().get();

    cameraData->cameraPos.x = playerData->position.x;
    cameraData->cameraPos.y = playerData->position.y;
    cameraData->cameraTarget = playerData->position;
    cameraData->bitFlag |= NS::CameraData::BF_DIRTY;

    if (cameraData->bitFlag & NS::CameraData::BF_DIRTY)
    {
        cameraData->cameraToClip = glm::ortho(-640.0f, 640.0f, -360.0f, 360.0f, 1.0f, 100.0f);
        cameraData->worldToCamera = glm::lookAt(cameraData->cameraPos, cameraData->cameraTarget, cameraData->cameraUp);
        cameraData->worldToClip = cameraData->cameraToClip * cameraData->worldToCamera;
        cameraData->bitFlag &= (~NS::CameraData::BF_DIRTY);
    }
}

void PlayerCameraSystem::receive(const entityx::ComponentAddedEvent<PlayerData>& evt)
{
    m_player = evt.entity;
}

void PlayerCameraSystem::receive(const entityx::ComponentAddedEvent<NS::CameraData>& evt)
{
    m_camera = evt.entity;
}
