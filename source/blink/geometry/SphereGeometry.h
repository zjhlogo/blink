/*!
 * \file SphereGeometry.h
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

class SphereGeometry : public BufferGeometry
{
public:
    SphereGeometry(float radius,
                   int rings = 8,
                   int segments = 16,
                   BufferAttributes::StockAttributes vertexAttribute = BufferAttributes::StockAttributes::Pos3Uv2Normal);
    virtual ~SphereGeometry();

private:
    float m_radius{};
    int m_rings{};
    int m_sections{};
};

NS_END
