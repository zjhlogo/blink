#pragma once
#include <entityx/entityx.h>
#include <camera/CameraData.h>
#include "MapRenderBlock.h"
#include "MapTileMaterial.h"

class MapRenderSystem : public entityx::System<MapRenderSystem>, public entityx::Receiver<MapRenderSystem>
{
public:
    MapRenderSystem();
    virtual ~MapRenderSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MapDataUpdateEvent& evt);
    void receive(const entityx::ComponentAddedEvent<blink::CameraData>& evt);

private:
    MapRenderBlock* m_renderBlock{};
    entityx::Entity m_camera;
    MapTileMaterial* m_material{};

};
