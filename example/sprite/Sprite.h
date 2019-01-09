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

    void setHead(int head) { m_head = head; };
    void setBody(int body) { m_body = body; };
    void setLeg(int leg) { m_leg = leg; };
    void setArm(int arm) { m_arm = arm; };

    int generateRenderBlock(SpriteRenderBlock& renderBlock, int depth);

private:
    int m_head{};
    int m_body{};
    int m_leg{};
    int m_arm{};

    int m_frameIndex{};
    float m_elapseTime{};

};
