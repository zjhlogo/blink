#pragma once
#include <render/geometry/BufferGeometry.h>
#include "MapData.h"
#include "Atlas.h"

class MapRenderBlock : public NS::BufferGeometry
{
public:
    static const int BLOCK_SIZE = 32;

    enum TileBorder
    {
        TOP = 1 << 0,
        RIGHT = 1 << 1,
        BOTTOM = 1 << 2,
        LEFT = 1 << 3,
        NUM_SET = 16,
    };

public:
    MapRenderBlock();
    virtual ~MapRenderBlock();

    void generateGeometry(const MapData* mapData, Atlas* atlas, const glm::ivec2& blockIndex);
    const glm::ivec2& getBlockIndex() const { return m_blockIndex; };

private:
    glm::ivec2 m_blockIndex;

};
