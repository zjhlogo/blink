/*!
 * \file InputSystem.h
 *
 * \author zjhlogo
 * \date 2019/11/15
 *
 * 
 */
#pragma once
#include <entityx/entityx.h>
#include "InputData.h"

NS_BEGIN

class InputSystem : public entityx::System<InputSystem>, public entityx::Receiver<InputSystem>
{
public:
    virtual ~InputSystem() {};

    void configure(entityx::EventManager &events) override;
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override;

    void receive(const MouseEvent& evt);

};

NS_END
