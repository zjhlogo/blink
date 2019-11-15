#pragma once
#include <entityx/entityx.h>
#include <camera/CameraData.h>
#include "MapRenderBlock.h"
#include "DiffuseMaterial.h"
#include "MapRenderBlock.h"

class Atlas;
class MapRenderSystem : public entityx::System<MapRenderSystem>, public entityx::Receiver<MapRenderSystem>
{
public:
    typedef std::vector<MapRenderBlock*> MapRenderBlockList;
    static const int MAX_RENDER_BLOCKS = 32;

public:
    MapRenderSystem();
    virtual ~MapRenderSystem();

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MapDataUpdateEvent& evt);
    void receive(const entityx::ComponentAddedEvent<NS::CameraData>& evt);

private:
    MapRenderBlock* checkOutBlock();
    void checkInBlock(MapRenderBlock* renderBlock);

    void checkInBlocksOutOfRange(const glm::ivec2& blockIndex);
    void generateBlocksInRange(const glm::ivec2& blockIndex);

private:
    MapRenderBlockList m_renderBlockInUsed;
    MapRenderBlockList m_renderBlockAvailable;
    int m_numBlocks{};

    entityx::Entity m_camera;
    DiffuseMaterial* m_material{};
    const MapData* m_mapData{};
    Atlas* m_atlas{};

    glm::ivec2 m_lastBlockIndex{ -1, -1 };
    glm::ivec2 m_blockRadius{ 3, 2 };

};
