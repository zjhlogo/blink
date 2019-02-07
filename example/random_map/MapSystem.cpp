#include "MapSystem.h"

MapSystem::~MapSystem()
{
}

void MapSystem::configure(entityx::EventManager & events)
{
    events.subscribe<entityx::ComponentAddedEvent<MapData>>(*this);
}

void MapSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{
    if (!m_mapEntity.valid()) return;

    auto mapData = m_mapEntity.component<MapData>().get();

    // notify map data updated
    if (mapData->bitFlag & MapData::BF_DIRTY)
    {
        MapDataUpdateEvent evt(mapData);
        events.emit(evt);

        mapData->bitFlag &= (~MapData::BF_DIRTY);
    }
}

void MapSystem::receive(const entityx::ComponentAddedEvent<MapData>& evt)
{
    m_mapEntity = evt.entity;
}

// TODO: modify map data
