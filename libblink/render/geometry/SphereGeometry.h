#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class SphereGeometry : public BufferGeometry
    {
    public:
        RTTI_DEF(SphereGeometry, BufferGeometry);

        SphereGeometry(float radius, int rings = 8, int segments = 16, BufferAttributes::StockAttributes vertexAttribute = BufferAttributes::StockAttributes::Pos3Uv2Normal);

    protected:
        virtual ~SphereGeometry();

    private:
        float m_radius{};
        int m_rings{};
        int m_sections{};

    };
}
