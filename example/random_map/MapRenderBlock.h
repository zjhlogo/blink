#pragma once
#include <render/geometry/BufferGeometry.h>
#include "MapData.h"

class MapRenderBlock : public blink::BufferGeometry
{
public:
    static const float TILE_SIZE;

public:
    RTTI_DEF(MapRenderBlock, blink::BufferGeometry);

    explicit MapRenderBlock(const MapData* mapData, const glm::ivec2& topLeft, const glm::ivec2& bottomRight);
    virtual ~MapRenderBlock() {};

    void generateGeometry();

private:
    const MapData* m_mapData{};
    glm::ivec2 m_topLeft;
    glm::ivec2 m_bottomRight;

};
