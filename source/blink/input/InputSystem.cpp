/*!
 * \file InputSystem.cpp
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 *
 */
#include "InputSystem.h"

#include <foundation/Log.h>

NS_BEGIN

void InputSystem::configure(entityx::EventManager& events)
{
    events.subscribe<MouseEvent>(*this);
}

void InputSystem::update(entityx::EntityManager& entities, entityx::EventManager& events, entityx::TimeDelta dt)
{
}

void InputSystem::receive(const MouseEvent& evt)
{
    LOGI("mouse event {0}", (int)evt.action);
}

NS_END
