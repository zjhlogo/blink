#include "Sprite.h"
#include <fmt/format.h>

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::update(float dt)
{
    m_elapseTime += dt;
    if (m_elapseTime > 0.1f)
    {
        m_frameIndex = (m_frameIndex + 1) % 20;
        m_elapseTime -= 0.1f;
    }
}

int Sprite::generateRenderBlock(SpriteRenderBlock& renderBlock, int depth)
{
    blink::tstring headTexFile = fmt::format("resource/Armor_Head_{0}.png", m_head);
    blink::tstring bodyTexFile = fmt::format("resource/Armor_Body_{0}.png", m_body);
    blink::tstring legTexFile = fmt::format("resource/Armor_Legs_{0}.png", m_leg);
    blink::tstring armTexFile = fmt::format("resource/Armor_Arm_{0}.png", m_arm);

    const auto& transform = getLocalToWorldTransform();
    // leg
    {
        auto group = renderBlock.checkOut(legTexFile);
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // body
    {
        auto group = renderBlock.checkOut(bodyTexFile);
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // head
    {
        auto group = renderBlock.checkOut(headTexFile);
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // arm
    {
        auto group = renderBlock.checkOut(armTexFile);
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    return depth + 1;
}
