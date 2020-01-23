/*!
 * \file InputData.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include <foundation/BaseTypes.h>
#include <foundation/BaseTypesGlm.h>

NS_BEGIN

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

    explicit MouseEvent(Action act, const glm::vec2& pos)
        :action(act)
        , mouseButton(MouseButtonLeft)
        , mousePos(pos)
        , modifyKey(0)
    {

    };

    explicit MouseEvent(Action act, MouseButton button, uint32 modKey)
        :action(act)
        , mouseButton(button)
        , mousePos()
        , modifyKey(modKey)
    {

    };

    Action action;
    MouseButton mouseButton;

    /// When mouse is moving, this is storing the mouse position
    ///
    /// When mouse is scrolling (using mouse wheel), this is storing
    /// the mouse wheel detail on mousePos.y. negative for backward 
    /// positive for foreword
    glm::vec2 mousePos;

    uint32 modifyKey;
};

struct KeyboardEvent
{
    explicit KeyboardEvent(int key_, int scancode_, int action_, int mods_)
        :key(key_)
        , scancode(scancode_)
        , action(action_)
        , mods(mods_)
    {

    }

    int key;
    int scancode;
    int action;
    int mods;
};

NS_END
