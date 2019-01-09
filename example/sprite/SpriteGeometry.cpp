#include "SpriteGeometry.h"
#include <geometries/BufferAttributes.h>
#include <render/GlConfig.h>
#include <glad/glad.h>

SpriteGeometry::SpriteGeometry()
{

}

SpriteGeometry::~SpriteGeometry()
{

}

bool SpriteGeometry::initialize()
{
    // setup index buffer
    std::vector<blink::uint16> indis;
    for (int i = 0; i < MAX_TRIANGLE; ++i)
    {
        blink::uint16 baseIndex = i * 4;
        indis.push_back(baseIndex + 0);
        indis.push_back(baseIndex + 2);
        indis.push_back(baseIndex + 1);
        indis.push_back(baseIndex + 1);
        indis.push_back(baseIndex + 2);
        indis.push_back(baseIndex + 3);
    }

    uploadIndexBuffer(indis.data(), indis.size());

    // setup vertex buffer
    uploadVertexBuffer(blink::BufferAttributes::fromStock(blink::BufferAttributes::StockAttributes::Pos3Uv2), nullptr, 0);

    return true;
}

void SpriteGeometry::updateVertex(const void* bufferData, blink::uint32 bufferSize)
{
    // Bind the VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    GL_ERROR_CHECK();

    // Set the buffer's data
    if (bufferSize > 0 && bufferData)
    {
        glBufferData(GL_ARRAY_BUFFER, bufferSize, bufferData, GL_DYNAMIC_DRAW);
        GL_ERROR_CHECK();
    }
}
