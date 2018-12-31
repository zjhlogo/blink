#pragma once
#include "../core/Component.h"

namespace blink
{
    class MouseEvent : public Event
    {
    public:
        enum class Action
        {
            Move,
            ButtonDown,
            ButtonUp,
        };

        enum MouseButton
        {
            MOUSE_BUTTON_1 = 0,
            MOUSE_BUTTON_2 = 1,
            MOUSE_BUTTON_3 = 2,
            MOUSE_BUTTON_4 = 3,
            MOUSE_BUTTON_5 = 4,
            MOUSE_BUTTON_6 = 5,
            MOUSE_BUTTON_7 = 6,
            MOUSE_BUTTON_8 = 7,

            MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
            MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
            MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_3,
        };

        static const uint32 MOD_SHIFT = 1 << 0;
        static const uint32 MOD_CONTROL = 1 << 1;
        static const uint32 MOD_ALT = 1 << 2;
        static const uint32 MOD_SUPER = 1 << 3;

    public:
        MouseEvent(Action act) :action(act) {};

    public:
        Action action{ Action::Move };
        MouseButton mouseButton{};
        glm::ivec2 mousePos;
        uint32 modifyKey{};

    };

    class MouseComponent : public Component
    {
    public:
        RTTI_DEF(MouseComponent, Component);

        virtual bool initialize(Entity* pEntity) override;
        virtual void terminate() override;

    };
}
