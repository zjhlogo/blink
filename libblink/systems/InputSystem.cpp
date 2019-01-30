#include "InputSystem.h"
#include <Log.h>

namespace blink
{
    void InputSystem::configure(entityx::EventManager & events)
    {
        events.subscribe<MouseEvent>(*this);
    }

    void InputSystem::update(entityx::EntityManager & entities, entityx::EventManager & events, entityx::TimeDelta dt)
    {

    }

    void InputSystem::receive(const MouseEvent & evt)
    {
        LOGI("mouse event {0}", (int)evt.action);
    }
}
