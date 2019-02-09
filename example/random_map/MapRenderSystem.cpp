#include "MapRenderSystem.h"

void MapRenderSystem::configure(entityx::EventManager & events)
{
    events.subscribe<MapDataUpdateEvent>(*this);

    // prepare render block pool
    m_renderBlocks.resize(MAX_RENDER_BLOCKS);
    for (int i = 0; i < MAX_RENDER_BLOCKS; ++i)
    {
        m_renderBlocks[i] = new MapRenderBlock()
    }
}

void MapRenderSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
{

}

void MapRenderSystem::receive(const MapDataUpdateEvent & evt)
{
    // TODO: rebuild map render block
}
