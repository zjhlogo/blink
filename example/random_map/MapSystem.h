#pragma once
#include <entityx/entityx.h>
#include "MapData.h"

class MapSystem : public entityx::System<MapSystem>, public entityx::Receiver<MapSystem>
{
public:
    virtual ~MapSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const entityx::ComponentAddedEvent<MapData>& evt);

private:
    entityx::Entity m_mapEntity;

};
