#include "MapRenderSystem.h"

void MapRenderSystem::configure(entityx::EventManager & events)
{
    events.subscribe<MapDataUpdateEvent>(*this);
    // TODO: prepare render block pool
}

void MapRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{

}

void MapRenderSystem::receive(const MapDataUpdateEvent & evt)
{
    // TODO: rebuild map render block
}
