
/*********************************************************************
 * \file   UserCommandSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/02/2022
 *********************************************************************/
#pragma once

#include <core/base/ILogicalSystem.h>

class UserCommandSystem : public blink::ILogicalSystem
{
public:
    UserCommandSystem();

    virtual bool initialize(flecs::world& world) override;
    virtual void terminate(flecs::world& world) override;

    virtual void framePreUpdate(flecs::world& world) override;
    virtual void framePostUpdate(flecs::world& world) override;

    void renderUi();

private:
    bool m_pushMeClicked{};

};
