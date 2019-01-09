#include "SpriteGeometry.h"
#include <geometries/BufferAttributes.h>

SpriteGeometry::SpriteGeometry(const glm::vec2 & spriteSize, int row, int col)
{
    m_spriteSize = spriteSize;
    m_row = row;
    m_col = col;

    float S = 1.0f / col;
    float T = 1.0f / row;

    // setup vertex buffer
    std::vector<blink::VertexPos3Uv2> verts;
    for (int y = 0; y < row; ++y)
    {
        for (int x = 0; x < col; ++x)
        {
            verts.push_back({ -0.5f*spriteSize.x, -0.5f*spriteSize.y, 0.0f, x*S, y*T });
            verts.push_back({ +0.5f*spriteSize.x, -0.5f*spriteSize.y, 0.0f, (x + 1)*S, y*T });
            verts.push_back({ -0.5f*spriteSize.x, +0.5f*spriteSize.y, 0.0f, x*S, (y + 1)*T });
            verts.push_back({ +0.5f*spriteSize.x, +0.5f*spriteSize.y, 0.0f, (x + 1)*S, (y + 1)*T });
        }
    }

    uploadVertexBuffer(blink::BufferAttributes::fromStock(blink::BufferAttributes::StockAttributes::Pos3Uv2), verts.data(), sizeof(verts[0]) * verts.size());

    // setup index buffer
    std::vector<blink::uint16> indis;
    indis.push_back(0);
    indis.push_back(2);
    indis.push_back(1);
    indis.push_back(1);
    indis.push_back(2);
    indis.push_back(3);
    uploadIndexBuffer(indis.data(), indis.size());
}

SpriteGeometry::~SpriteGeometry()
{

}
