#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class PlaneGeometry : public BufferGeometry
    {
    public:
        enum class Facing
        {
            PositiveX,
            NegativeX,
            PositiveY,
            NegativeY,
            PositiveZ,
            NegativeZ,
        };

    public:
        RTTI_DEF(PlaneGeometry, BufferGeometry);

        PlaneGeometry(float width, float height, Facing facing, int widthSegments = 1, int heightSegments = 1);
        virtual ~PlaneGeometry();

    private:
        float m_width{};
        float m_height{};
        Facing m_facing{ Facing::PositiveX };
        int m_widthSegments{};
        int m_heightSegments{};

    };
}
