#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class PlaneGeometry : public BufferGeometry
    {
    public:
        enum class PlaneFaceDirection
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

        PlaneGeometry(float width, float height, PlaneFaceDirection dir, int widthSegments = 1, int heightSegments = 1);
        virtual ~PlaneGeometry();

    private:
        float m_width{};
        float m_height{};
        PlaneFaceDirection m_dir{ PlaneFaceDirection::PositiveX };
        int m_widthSegments{};
        int m_heightSegments{};

    };
}
