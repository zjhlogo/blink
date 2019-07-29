/*!
 * \file BoxGeometry.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#pragma once
#include "BufferGeometry.h"
#include "BufferAttributes.h"

namespace blink
{
    class BoxGeometry : public BufferGeometry
    {
    public:
        BoxGeometry(float width,
            float height,
            float depth,
            int widthSegments = 1,
            int heightSegments = 1,
            int depthSegments = 1,
            BufferAttributes::StockAttributes vertexAttribute = BufferAttributes::StockAttributes::Pos3Uv2Normal);
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
