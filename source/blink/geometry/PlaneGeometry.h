/*!
 * \file PlaneGeometry.h
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#pragma once
#include <render_base/BufferAttributes.h>
#include <render_base/BufferGeometry.h>

NS_BEGIN

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
    PlaneGeometry(float width,
                  float height,
                  Facing facing,
                  int widthSegments = 1,
                  int heightSegments = 1,
                  BufferAttributes::StockAttributes vertexAttribute = BufferAttributes::StockAttributes::Pos3Uv2Normal);
    virtual ~PlaneGeometry();

private:
    float m_width{};
    float m_height{};
    Facing m_facing{ Facing::PositiveX };
    int m_widthSegments{};
    int m_heightSegments{};
};

NS_END
