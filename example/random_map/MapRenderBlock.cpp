#include "MapRenderBlock.h"
#include <render/geometry/BufferAttributes.h>

const float MapRenderBlock::TILE_SIZE = 8.0f;

MapRenderBlock::MapRenderBlock(const MapData * mapData, const glm::ivec2 & topLeft, const glm::ivec2 & bottomRight)
    :m_mapData(mapData)
    , m_topLeft(topLeft)
    , m_bottomRight(bottomRight)
{
}

void MapRenderBlock::generateGeometry()
{
    std::vector<blink::VertexPos3Uv2> verts;
    std::vector<blink::uint16> indis;

    int numVerts = 0;
    for (int y = m_topLeft.y; y <= m_bottomRight.y; ++y)
    {
        for (int x = m_topLeft.x; x <= m_bottomRight.x; ++x)
        {
            int index = y * m_mapData->width + x;
            // TODO: use index to get tile type

            verts.push_back({ x * TILE_SIZE, y * TILE_SIZE, 0.0f, 0.0f, 0.0f });
            verts.push_back({ x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, 0.0f, 0.0f, 0.0f });
            verts.push_back({ x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, 0.0f, 0.0f, 0.0f });
            verts.push_back({ x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE, 0.0f, 0.0f, 0.0f });

            indis.push_back(numVerts + 0);
            indis.push_back(numVerts + 2);
            indis.push_back(numVerts + 1);
            indis.push_back(numVerts + 1);
            indis.push_back(numVerts + 2);
            indis.push_back(numVerts + 3);

            numVerts += 4;
        }
    }

    uploadVertexBuffer(blink::BufferAttributes::StockAttributes::Pos3Uv2, verts.data(), sizeof(verts[0])* numVerts);
    uploadIndexBuffer(indis.data(), indis.size());
}
