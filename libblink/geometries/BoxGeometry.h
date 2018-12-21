#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class BoxGeometry : public BufferGeometry
    {
    public:
        RTTI_DEF(BoxGeometry, BufferGeometry);

        BoxGeometry(float width, float height, float depth, int widthSegments = 1, int heightSegments = 1, int depthSegments = 1);
        virtual ~BoxGeometry();

    private:
        float m_width{};
        float m_height{};
        float m_depth{};
        int m_widthSegments{};
        int m_heightSegments{};
        int m_depthSegments{};

    };
}
