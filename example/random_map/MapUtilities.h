#pragma once
#include <BaseType.h>
#include "MapData.h"

class MapUtilities
{
public:
    static const float TILE_SIZE;

    static glm::vec3 getWorldPos(const MapData* mapData, const glm::ivec2& gridPos);
    static glm::ivec2 getGridPos(const MapData* mapData, const glm::vec3& worldPos);
    static glm::ivec2 findGroundGridPos(const MapData* mapData, const glm::ivec2& gridPos);

    static bool isWalkable(const MapData* mapData, const glm::ivec2& gridPos);
    static bool crossable(const MapData * mapData, const glm::ivec2 & prevGridPos, const glm::ivec2 & nextGridPos);

};
