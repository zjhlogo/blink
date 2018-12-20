#pragma once
#include "Geometry.h"

namespace blink
{
    class BufferAttributes;

    class BufferGeometry : public Geometry
    {
    public:
        RTTI_DEF(BufferGeometry, Geometry);

        BufferGeometry();
        virtual ~BufferGeometry();

        uint32 getVertexBufferId() const { return m_vertexBufferId; };
        uint32 getIndexBufferId() const { return m_indexBufferId; };
        uint32 getNumIndex() const { return m_numIndex; };

        const BufferAttributes* getBufferAttributes() const { return m_vertexBufferAttribute; };
        bool uploadVertexBuffer(BufferAttributes* bufferAttribute, const void* bufferData, uint32 bufferSize);
        bool uploadIndexBuffer(const uint16* bufferData, uint32 numIndex);

    private:
        void destroyVertexBuffer();
        void destroyIndexBuffer();

    private:
        BufferAttributes* m_vertexBufferAttribute{};

        uint32 m_vertexBufferId{};
        uint32 m_indexBufferId{};
        uint32 m_numIndex{};

    };
}