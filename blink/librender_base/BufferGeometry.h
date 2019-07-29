/*!
 * \file BufferGeometry.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include "Geometry.h"
#include "BufferAttributes.h"
#include "BufferObject.h"
#include "VertexBuffer.h"

namespace blink
{
    class BufferGeometry : public Geometry
    {
    public:
        BufferGeometry();
        virtual ~BufferGeometry();

        bool uploadVertexBuffer(BufferAttributes::StockAttributes stock, const void* bufferData, uint32 bufferSize, BufferObject::Usage usage = BufferObject::Usage::StaticDraw);
        bool uploadVertexBuffer(BufferAttributes* attributes, const void* bufferData, uint32 bufferSize, BufferObject::Usage usage = BufferObject::Usage::StaticDraw);
        bool uploadIndexBuffer(const uint16* bufferData, uint32 numIndex, BufferObject::Usage usage = BufferObject::Usage::StaticDraw);

    protected:
        void destroyVertexBuffer();
        void destroyIndexBuffer();

    protected:
        VertexBuffer* m_vertexBuffer{};
        BufferObject* m_indexBuffer{};

    };
}
