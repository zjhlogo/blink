
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

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void framePreUpdate() override;
    virtual void framePostUpdate() override;

    void renderUi();

private:
    bool m_pushMeClicked{};

};
