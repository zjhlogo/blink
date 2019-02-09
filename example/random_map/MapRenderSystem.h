#pragma once
#include <entityx/entityx.h>
#include "MapData.h"
#include "MapRenderBlock.h"

class MapRenderSystem : public entityx::System<MapRenderSystem>, public entityx::Receiver<MapRenderSystem>
{
public:
    static const int MAX_RENDER_BLOCKS = 16;

public:
    virtual ~MapRenderSystem() {};

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MapDataUpdateEvent& evt);

private:
    std::vector<MapRenderBlock*> m_renderBlocks;

};
