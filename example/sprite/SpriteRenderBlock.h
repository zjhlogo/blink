#pragma once
#include <textures/Texture2D.h>
#include <geometries/BufferAttributes.h>

class SpriteQuad
{
public:
    glm::vec4 verts[4];
    glm::vec2 uvs[4];
    int depth{};

};

class SpriteQuadGroup
{
public:
    void addQuad(float width, float height, int row, int col, int index, const glm::mat4& transform);
    void addQuad(const blink::tstring& texFile, const glm::mat4& transform);

public:
    blink::tstring texFile;
    std::vector<SpriteQuad> quads;

};

class SpriteRenderBlock
{
public:
    SpriteRenderBlock();
    ~SpriteRenderBlock();

    SpriteQuadGroup* checkOut(const blink::tstring& texFile);
    void reset();

public:
    std::vector<SpriteQuadGroup*> m_spriteQuadGroupList;

};
