#pragma once
#include "../Geometry.h"

namespace blink
{
    class BufferAttributes;

    class BufferGeometry : public Geometry
    {
    public:
        BufferGeometry();
        ~BufferGeometry();

        const BufferAttributes* getVertexBufferAttributes() const { return m_vertexBufferAttribute; };
        uint32 getVertexBufferId() const { return m_vertexBufferId; };
        uint32 getIndexBufferId() const { return m_indexBufferId; };

        bool uploadVertexBuffer(const BufferAttributes* bufferAttribute, const void* bufferData, uint32 bufferSize);
        bool uploadIndexBuffer(const uint16* bufferData, uint32 numIndex);

    private:
        const BufferAttributes* m_vertexBufferAttribute{};

        uint32 m_vertexBufferId{};
        uint32 m_indexBufferId{};

    };
}
