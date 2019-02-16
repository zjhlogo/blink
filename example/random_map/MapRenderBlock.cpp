#include "MapRenderBlock.h"
#include <render/geometry/BufferAttributes.h>

const float MapRenderBlock::TILE_SIZE = 16.0f;

MapRenderBlock::MapRenderBlock(const MapData * mapData, const glm::ivec2 & topLeft, const glm::ivec2 & bottomRight)
    :m_mapData(mapData)
    , m_topLeft(topLeft)
    , m_bottomRight(bottomRight)
{
    // TODO: move it to a better place
    for (int i = 0; i < NUM_SET; ++i)
    {
        m_rockUvs[i].s = (m_rockUvs[i].s * 18 + 8) / 288;
        m_rockUvs[i].t = (270.0f - m_rockUvs[i].t * 18 - 8) / 270.0f;
    }
}

void MapRenderBlock::generateGeometry()
{
    std::vector<blink::VertexPos3Uv2> verts;
    std::vector<blink::uint16> indis;

    float hu = 16.0f / 288.0f * 0.5f;
    float hv = 16.0f / 270.0f * 0.5f;
    float mapHeight = m_mapData->height * TILE_SIZE;

    int numVerts = 0;
    for (int y = m_topLeft.y; y < m_bottomRight.y; ++y)
    {
        for (int x = m_topLeft.x; x < m_bottomRight.x; ++x)
        {
            int index = y * m_mapData->width + x;

            // TODO: use index to get tile type
            if (m_mapData->buffer[index] > 0)
            {
                int indexTop = (y - 1) * m_mapData->width + x;
                int indexBottom = (y + 1) * m_mapData->width + x;
                int indexLeft = y * m_mapData->width + x - 1;
                int indexRight = y * m_mapData->width + x + 1;

                int uvIndex = 0;
                if (m_mapData->buffer[indexTop] == 0) uvIndex |= TOP;
                if (m_mapData->buffer[indexRight] == 0) uvIndex |= RIGHT;
                if (m_mapData->buffer[indexBottom] == 0) uvIndex |= BOTTOM;
                if (m_mapData->buffer[indexLeft] == 0) uvIndex |= LEFT;

                //verts.push_back({ x * TILE_SIZE, y * TILE_SIZE, 0.0f, 0.0f, 0.0f });
                //verts.push_back({ x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE, 0.0f, 1.0f, 0.0f });
                //verts.push_back({ x * TILE_SIZE, y * TILE_SIZE + TILE_SIZE, 0.0f, 0.0f, 1.0f });
                //verts.push_back({ x * TILE_SIZE + TILE_SIZE, y * TILE_SIZE + TILE_SIZE, 0.0f, 1.0f, 1.0f });

                verts.push_back({ x * TILE_SIZE, mapHeight - y * TILE_SIZE - TILE_SIZE, 0.0f, m_rockUvs[uvIndex].s - hu, m_rockUvs[uvIndex].t - hv });
                verts.push_back({ x * TILE_SIZE + TILE_SIZE, mapHeight - y * TILE_SIZE - TILE_SIZE, 0.0f, m_rockUvs[uvIndex].s + hu, m_rockUvs[uvIndex].t - hv });
                verts.push_back({ x * TILE_SIZE, mapHeight - y * TILE_SIZE + TILE_SIZE - TILE_SIZE, 0.0f, m_rockUvs[uvIndex].s - hu, m_rockUvs[uvIndex].t + hv });
                verts.push_back({ x * TILE_SIZE + TILE_SIZE, mapHeight - y * TILE_SIZE + TILE_SIZE - TILE_SIZE, 0.0f, m_rockUvs[uvIndex].s + hu, m_rockUvs[uvIndex].t + hv });

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

    uploadVertexBuffer(blink::BufferAttributes::StockAttributes::Pos3Uv2, verts.data(), sizeof(verts[0])* numVerts);
    uploadIndexBuffer(indis.data(), indis.size());
}
