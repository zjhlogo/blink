#pragma once

#include <geometries/BufferGeometry.h>

class SpriteGeometry : public blink::BufferGeometry
{
public:
    static const int MAX_TRIANGLE = 100;

public:
    SpriteGeometry();
    virtual ~SpriteGeometry();

    bool initialize();
    void updateVertex(const void* bufferData, blink::uint32 bufferSize);

private:
    glm::vec2 m_spriteSize;
    int m_row{};
    int m_col{};

};
