/*********************************************************************
 * \file   UserCommandSystem.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/02/2022
 *********************************************************************/
#pragma once

#include <core/systems/ILogicalSystem.h>

class UserCommandSystem final : public blink::ILogicalSystem
{
public:
    bool initialize() override;
    void terminate() override;

    void framePreUpdate() override;
    void framePostUpdate() override;

    void onGui();

private:
    bool m_pushMeClicked{};
};
