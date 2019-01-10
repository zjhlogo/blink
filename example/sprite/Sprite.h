#pragma once
#include <objects/Object.h>
#include "SpriteRenderBlock.h"

class Sprite : public blink::Object
{
public:
    RTTI_DEF(Sprite, blink::Object);

    Sprite();
    virtual ~Sprite();

    void update(float dt);

    int generateRenderBlock(SpriteRenderBlock& renderBlock, int depth);

public:
    bool m_male{ true };
    int m_headId{ 1 };
    int m_bodyId{ 1 };
    int m_legId{ 1 };
    int m_armId{ 1 };

    int m_handsOffId{};
    int m_handsOnId{};

    int m_backId{};
    int m_frontId{};

    int m_shoeId{};
    int m_waistId{};
    int m_wingId{};
    int m_shieldId{};
    int m_neckId{};
    int m_faceId{};
    int m_balloonId{};

private:
    int m_frameIndex{};
    float m_elapseTime{};

};
