
/*********************************************************************
 * \file   SinglePendulumSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <core/base/ILogicalSystem.h>

class SinglePendulumSystem : public blink::ILogicalSystem
{
public:
    SinglePendulumSystem() = default;

    virtual bool initialize() override;
    virtual void terminate() override;
};
