#include "MapUtilities.h"

const float MapUtilities::TILE_SIZE = 16.0f;

glm::vec3 MapUtilities::getWorldPos(const MapData* mapData, const glm::ivec2 & gridPos)
{
    return glm::vec3((gridPos.x - mapData->origin.x) * TILE_SIZE, (gridPos.y - mapData->origin.y) * TILE_SIZE, 0.0f);
}

glm::ivec2 MapUtilities::getGridPos(const MapData* mapData, const glm::vec3 & worldPos)
{
    return glm::ivec2(static_cast<int>(worldPos.x / TILE_SIZE) + mapData->origin.x, static_cast<int>(worldPos.y / TILE_SIZE) + mapData->origin.y);
}

glm::ivec2 MapUtilities::findGroundGridPos(const MapData * mapData, const glm::ivec2 & gridPos)
{
    int index = gridPos.y * mapData->size.x + gridPos.x;
    if (mapData->buffer[index] == 0)
    {
        glm::ivec2 tempGridPos = gridPos;
        while (tempGridPos.y > 1)
        {
            int tempIndex = (tempGridPos.y - 1) * mapData->size.x + tempGridPos.x;
            if (mapData->buffer[tempIndex] != 0) break;
            --tempGridPos.y;
        }

        return tempGridPos;
    }

    glm::ivec2 tempGridPos = gridPos;
    while (tempGridPos.y < mapData->size.y - 1)
    {
        ++tempGridPos.y;
        int tempIndex = tempGridPos.y * mapData->size.x + tempGridPos.x;
        if (mapData->buffer[tempIndex] == 0) break;
    }

    return tempGridPos;
}

bool MapUtilities::isWalkable(const MapData * mapData, const glm::ivec2 & gridPos)
{
    int index = gridPos.y * mapData->size.x + gridPos.x;
    return (mapData->buffer[index] == 0);
}

bool MapUtilities::crossable(const MapData * mapData, const glm::ivec2 & prevGridPos, const glm::ivec2 & nextGridPos)
{
    int index0 = (nextGridPos.y + 2) * mapData->size.x + nextGridPos.x;
    int index1 = (nextGridPos.y + 3) * mapData->size.x + nextGridPos.x;
    int index2 = (nextGridPos.y + 4) * mapData->size.x + nextGridPos.x;

    return (mapData->buffer[index0] == 0) && (mapData->buffer[index1] == 0) && (mapData->buffer[index2] == 0);
}
