#pragma once
#include <entityx/entityx.h>
#include <camera/CameraData.h>
#include "PlayerData.h"

class PlayerCameraSystem : public entityx::System<PlayerCameraSystem>, public entityx::Receiver<PlayerCameraSystem>
{
public:
    virtual ~PlayerCameraSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const entityx::ComponentAddedEvent<PlayerData>& evt);
    void receive(const entityx::ComponentAddedEvent<NS::CameraData>& evt);

private:
    entityx::Entity m_player;
    entityx::Entity m_camera;

};
