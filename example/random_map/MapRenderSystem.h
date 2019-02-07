#pragma once
#include <entityx/entityx.h>
#include "MapData.h"

class MapRenderSystem : public entityx::System<MapRenderSystem>, public entityx::Receiver<MapRenderSystem>
{
public:
    virtual ~MapRenderSystem() {};

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MapDataUpdateEvent& evt);

};
