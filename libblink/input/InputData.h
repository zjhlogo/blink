#pragma once
#include <BaseType.h>

namespace blink
{
	struct MouseEvent
    {
        enum Action
        {
            Move,
            ButtonDown,
            ButtonUp,
            Scroll,
        };

        enum MouseButton
        {
            MouseButton1 = 0,
            MouseButton2 = 1,
            MouseButton3 = 2,
            MouseButton4 = 3,
            MouseButton5 = 4,
            MouseButton6 = 5,
            MouseButton7 = 6,
            MouseButton8 = 7,

            MouseButtonLeft = MouseButton1,
            MouseButtonRight = MouseButton2,
            MouseButtonMiddle = MouseButton3,
        };

        enum KeyModify
        {
            ModifyShift = 1 << 0,
            ModifyControl = 1 << 1,
            ModifyAlt = 1 << 2,
            ModifySuper = 1 << 3,
        };

        MouseEvent(Action act, const glm::vec2& pos) :action(act), mousePos(pos) {};
        MouseEvent(Action act, MouseButton button, uint32 modKey) :action(act), mouseButton(button), modifyKey(modKey) {};

        Action action{ Action::Move };
        MouseButton mouseButton{};
        glm::vec2 mousePos{};
        uint32 modifyKey{};
    };
}
