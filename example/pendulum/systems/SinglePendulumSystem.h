/*********************************************************************
 * \file   SinglePendulumSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   4/15 2022
 *********************************************************************/
#pragma once

#include <core/systems/ILogicalSystem.h>

class SinglePendulumSystem final : public blink::ILogicalSystem
{
public:
    SinglePendulumSystem() = default;

    bool initialize() override;
    void terminate() override;

    void framePostUpdate() override;
};
