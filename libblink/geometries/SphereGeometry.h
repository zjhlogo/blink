#pragma once
#include "BufferGeometry.h"

namespace blink
{
    class SphereGeometry : public BufferGeometry
    {
    public:
        RTTI_DEF(SphereGeometry, BufferGeometry);

        SphereGeometry(float radius, int rings = 3, int segments = 3);
        virtual ~SphereGeometry();

    private:
        float m_radius{};
        int m_rings{};
        int m_sections{};

    };
}
