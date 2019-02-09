#pragma once
#include "Geometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class BufferGeometry : public Geometry
    {
    public:
        RTTI_DEF(BufferGeometry, Geometry);

        uint32 getVertexArrayObjectId() const { return m_vertexArrayObjectId; };
        //uint32 getVertexBufferId() const { return m_vertexBufferId; };
        uint32 getIndexBufferId() const { return m_indexBufferId; };
        uint32 getNumIndex() const { return m_numIndex; };

        const BufferAttributes* getBufferAttributes() const { return m_vertexBufferAttribute; };
        bool uploadVertexBuffer(BufferAttributes::StockAttributes stockAttr, const void* bufferData, uint32 bufferSize);
        bool uploadVertexBuffer(BufferAttributes* bufferAttribute, const void* bufferData, uint32 bufferSize);
        bool uploadIndexBuffer(const uint16* bufferData, uint32 numIndex);

    protected:
        BufferGeometry() {};
        virtual ~BufferGeometry();

        void destroyVertexArrayObject();
        void destroyVertexBuffer();
        void destroyIndexBuffer();

    protected:
        BufferAttributes* m_vertexBufferAttribute{};

        uint32 m_vertexArrayObjectId{};
        uint32 m_vertexBufferId{};
        uint32 m_indexBufferId{};
        uint32 m_numIndex{};

    };
}
