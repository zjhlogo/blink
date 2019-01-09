#include "SpriteRenderBlock.h"

void SpriteQuadGroup::addQuad(float width, float height, int row, int col, int index, const glm::mat4& transform)
{
    float texWidth = col * width;
    float texHeight = row * height;

    float ds = 1.0f / col;
    float dt = 1.0f / row;

    int x = index % col;
    int y = index / col;

    quads.resize(quads.size() + 1);
    auto& quad = *quads.rbegin();

    quad.uvs[0] = { x * ds, (row - y - 1) * dt };
    quad.uvs[1] = { (x + 1) * ds, (row - y - 1) * dt };
    quad.uvs[2] = { x * ds, (row - y) * dt };
    quad.uvs[3] = { (x + 1) * ds, (row - y) * dt };

    quad.verts[0] = transform * glm::vec4(-0.5f * width, -0.5f * height, 0.0f, 1.0f);
    quad.verts[1] = transform * glm::vec4(+0.5f * width, -0.5f * height, 0.0f, 1.0f);
    quad.verts[2] = transform * glm::vec4(-0.5f * width, +0.5f * height, 0.0f, 1.0f);
    quad.verts[3] = transform * glm::vec4(+0.5f * width, +0.5f * height, 0.0f, 1.0f);
}

SpriteRenderBlock::SpriteRenderBlock()
{

}

SpriteRenderBlock::~SpriteRenderBlock()
{
    reset();
}

SpriteQuadGroup* SpriteRenderBlock::checkOut(const blink::tstring & texFile)
{
    for (auto& group : m_spriteQuadGroupList)
    {
        if (group->texFile == texFile) return group;
    }

    SpriteQuadGroup* group = new SpriteQuadGroup();
    group->texFile = texFile;
    m_spriteQuadGroupList.push_back(group);

    return group;
}

void SpriteRenderBlock::reset()
{
    for (auto& group : m_spriteQuadGroupList)
    {
        SAFE_DELETE(group);
    }

    m_spriteQuadGroupList.clear();
}
