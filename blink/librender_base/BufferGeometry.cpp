/*!
 * \file BufferGeometry.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "BufferGeometry.h"
#include "RenderModule.h"

NS_BEGIN

BufferGeometry::BufferGeometry()
{

}

BufferGeometry::~BufferGeometry()
{
    destroyVertexBuffer();
    destroyIndexBuffer();
}

bool BufferGeometry::uploadVertexBuffer(BufferAttributes::StockAttributes stock, const void* bufferData, uint32 bufferSize, BufferObject::Usage usage /* = BufferObject::Usage::StaticDraw */)
{
    auto attributes = BufferAttributes::fromStock(stock);
    return uploadVertexBuffer(attributes, bufferData, bufferSize, usage);
}

bool BufferGeometry::uploadVertexBuffer(BufferAttributes* attributes, const void* bufferData, uint32 bufferSize, BufferObject::Usage usage /* = BufferObject::Usage::StaticDraw */)
{
    if (!attributes) return false;

    // create buffer object
    if (!m_vertexBuffer)
    {
        m_vertexBuffer = RenderModule::createVertexBuffer(attributes);
        if (!m_vertexBuffer) return false;
    }

    return m_vertexBuffer->uploadBuffer(bufferData, bufferSize, usage);
}

bool BufferGeometry::uploadIndexBuffer(const uint16* bufferData, uint32 numIndex, BufferObject::Usage usage /* = BufferObject::Usage::StaticDraw */)
{
    // create buffer object
    if (!m_indexBuffer)
    {
        m_indexBuffer = RenderModule::createBufferObject(BufferObject::BufferType::ElementArrayBuffer);
        if (!m_indexBuffer) return false;
    }

    return m_indexBuffer->uploadBufferData(bufferData, numIndex * sizeof(uint16), usage);
}

void BufferGeometry::destroyVertexBuffer()
{
    RenderModule::destroyVertexBuffer(m_vertexBuffer);
    m_vertexBuffer = nullptr;
}

void BufferGeometry::destroyIndexBuffer()
{
    RenderModule::destroyBufferObject(m_indexBuffer);
    m_indexBuffer = nullptr;
}

NS_END
