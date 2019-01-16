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
    //m_elapseTime += dt;
    //if (m_elapseTime > 0.1f)
    //{
    //    m_frameIndex = (m_frameIndex + 1) % 20;
    //    m_elapseTime -= 0.1f;
    //}
}

int Sprite::generateRenderBlock(SpriteRenderBlock& renderBlock, int depth)
{
    const auto& transform = getLocalToWorldTransform();

    // back
    if (m_backId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/back/Acc_Back_{0}.png", m_backId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // wing
    if (m_wingId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/wing/Wings_{0}.png", m_wingId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // leg
    if (m_legId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/leg/Armor_Legs_{0}.png", m_legId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // body
    if (m_bodyId > 0)
    {
        if (m_male)
        {
            auto group = renderBlock.checkOut(fmt::format("resource/armor/body_male/Armor_Body_{0}.png", m_bodyId));
            group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
        }
        else
        {
            auto group = renderBlock.checkOut(fmt::format("resource/armor/body_female/Female_Body_{0}.png", m_bodyId));
            group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
        }
    }

    // head
    if (m_headId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/head/Armor_Head_{0}.png", m_headId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // neck
    if (m_neckId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/neck/Acc_Neck_{0}.png", m_neckId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // face
    if (m_faceId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/face/Acc_Face_{0}.png", m_faceId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // waist
    if (m_waistId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/waist/Acc_Waist_{0}.png", m_waistId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // arm
    if (m_armId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/arm/Armor_Arm_{0}.png", m_armId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // hands
    if (m_handsOffId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/hands_off/Acc_HandsOff_{0}.png", m_handsOffId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }
    if (m_handsOnId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/hands_on/Acc_HandsOn_{0}.png", m_handsOnId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // front
    if (m_frontId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/front/Acc_Front_{0}.png", m_frontId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // shoe
    if (m_shoeId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/shoe/Acc_Shoes_{0}.png", m_shoeId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // shield
    if (m_shieldId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/shield/Acc_Shield_{0}.png", m_shieldId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // balloon
    if (m_balloonId > 0)
    {
        auto group = renderBlock.checkOut(fmt::format("resource/armor/balloon/Acc_Balloon_{0}.png", m_balloonId));
        group->addQuad(40.0f, 56.0f, 20, 1, m_frameIndex, transform);
    }

    // weapon
    if (m_weaponId > 0)
    {
        blink::tstring texFile = fmt::format("resource/item/Item_{0}.png", m_weaponId);
        auto group = renderBlock.checkOut(texFile);
        group->addQuad(texFile, transform);
    }

    return depth + 1;
}
