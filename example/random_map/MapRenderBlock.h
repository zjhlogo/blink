#pragma once
#include <render/geometry/BufferGeometry.h>
#include "MapData.h"

class MapRenderBlock : public blink::BufferGeometry
{
public:
    static const float TILE_SIZE;

    enum TileBorder
    {
        TOP = 1 << 0,
        RIGHT = 1 << 1,
        BOTTOM = 1 << 2,
        LEFT = 1 << 3,
        NUM_SET = 16,
    };

public:
    RTTI_DEF(MapRenderBlock, blink::BufferGeometry);

    explicit MapRenderBlock(const MapData* mapData, const glm::ivec2& topLeft, const glm::ivec2& bottomRight);
    virtual ~MapRenderBlock() {};

    void generateGeometry();

private:
    const MapData* m_mapData{};
    glm::ivec2 m_topLeft;
    glm::ivec2 m_bottomRight;

    glm::vec2 m_rockUvs[NUM_SET]
    {
        {1.0f, 1.0f},
        {1.0f, 0.0f},
        {4.0f, 0.0f},
        {1.0f, 3.0f},
        {1.0f, 2.0f},
        {6.0f, 4.0f},
        {1.0f, 4.0f},
        {12.0f, 0.0f},
        {0.0f, 0.0f},
        {0.0f, 3.0f},
        {5.0f, 0.0f},
        {6.0f, 0.0f},
        {0.0f, 4.0f},
        {9.0f, 0.0f},
        {6.0f, 3.0f},
        {9.0f, 3.0f},
    };
};
