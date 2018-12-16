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

        const BufferAttributes* getBufferAttributes() const { return m_vertexBufferAttribute; };
        bool uploadVertexBuffer(const BufferAttributes* bufferAttribute, const void* bufferData, uint32 bufferSize);
        bool uploadIndexBuffer(const uint16* bufferData, uint32 numIndex);

    private:
        void destroyVertexBuffer();
        void destroyIndexBuffer();

    private:
        const BufferAttributes* m_vertexBufferAttribute{};

        uint32 m_vertexBufferId{};
        uint32 m_indexBufferId{};

    };
}
