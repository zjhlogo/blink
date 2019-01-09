#pragma once

#include <geometries/BufferGeometry.h>

class SpriteGeometry : public blink::BufferGeometry
{
public:
    SpriteGeometry(const glm::vec2& spriteSize, int row, int col);
    virtual ~SpriteGeometry();

private:
    glm::vec2 m_spriteSize;
    int m_row{};
    int m_col{};

};
