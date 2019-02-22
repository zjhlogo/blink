#include "MapRenderBlock.h"
#include <glad/glad.h>
#include <render/geometry/BufferAttributes.h>
#include <algorithm>

const float MapRenderBlock::TILE_SIZE = 16.0f;

MapRenderBlock::MapRenderBlock()
{

}

MapRenderBlock::~MapRenderBlock()
{

}

void MapRenderBlock::generateGeometry(const MapData* mapData, Atlas* atlas, const glm::ivec2& blockIndex)
{
    m_blockIndex = blockIndex;

    std::vector<blink::VertexPos3Uv2> verts;
    std::vector<blink::uint16> indis;

    glm::ivec2 lb{ blockIndex.x * BLOCK_SIZE, blockIndex.y * BLOCK_SIZE };
    glm::ivec2 rt{ std::min(lb.x + BLOCK_SIZE, mapData->width - 1), std::min(lb.y + BLOCK_SIZE, mapData->height - 1) };

    if (lb.x == 0) ++lb.x;
    if (lb.y == 0) ++lb.y;

    float offsetX = -mapData->originX * TILE_SIZE;
    float offsetY = -mapData->originY * TILE_SIZE;

    int numVerts = 0;
    for (int y = lb.y; y < rt.y; ++y)
    {
        for (int x = lb.x; x < rt.x; ++x)
        {
            int index = y * mapData->width + x;
            auto tileValue = mapData->buffer[index];

            if (tileValue > 0)
            {
                int indexTop = (y + 1) * mapData->width + x;
                int indexBottom = (y - 1) * mapData->width + x;
                int indexLeft = y * mapData->width + x - 1;
                int indexRight = y * mapData->width + x + 1;

                int uvIndex = 0;
                if (mapData->buffer[indexTop] != tileValue) uvIndex |= TOP;
                if (mapData->buffer[indexRight] != tileValue) uvIndex |= RIGHT;
                if (mapData->buffer[indexBottom] != tileValue) uvIndex |= BOTTOM;
                if (mapData->buffer[indexLeft] != tileValue) uvIndex |= LEFT;

                // calculate tile index
                int tileIndex = 1;
                if (mapData->buffer[index] < 128) tileIndex = 0;
                tileIndex = tileIndex * 16 + uvIndex;
                auto piece = atlas->getPiece(tileIndex);

                verts.push_back({ offsetX + x * TILE_SIZE,             offsetY + y * TILE_SIZE,             0.0f, piece->uvs[0].s, piece->uvs[0].t });
                verts.push_back({ offsetX + x * TILE_SIZE + TILE_SIZE, offsetY + y * TILE_SIZE,             0.0f, piece->uvs[1].s, piece->uvs[1].t });
                verts.push_back({ offsetX + x * TILE_SIZE,             offsetY + y * TILE_SIZE + TILE_SIZE, 0.0f, piece->uvs[2].s, piece->uvs[2].t });
                verts.push_back({ offsetX + x * TILE_SIZE + TILE_SIZE, offsetY + y * TILE_SIZE + TILE_SIZE, 0.0f, piece->uvs[3].s, piece->uvs[3].t });

                indis.push_back(numVerts + 0);
                indis.push_back(numVerts + 2);
                indis.push_back(numVerts + 1);
                indis.push_back(numVerts + 1);
                indis.push_back(numVerts + 2);
                indis.push_back(numVerts + 3);

                numVerts += 4;
            }
        }
    }

    uploadVertexBuffer(blink::BufferAttributes::StockAttributes::Pos3Uv2, verts.data(), sizeof(verts[0])* numVerts, GL_STATIC_DRAW);
    uploadIndexBuffer(indis.data(), indis.size(), GL_STATIC_DRAW);
}
