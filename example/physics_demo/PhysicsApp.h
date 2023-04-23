/*********************************************************************
 * \file   PhysicsApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include "common/guis/IGuiWindow.h"

#include <blink/app.h>

namespace blink
{
    class JoltPhysicsSystem;
}

class PhysicsApp final : public blink::IApp, public IGuiWindow
{
public:
    void onGui() override;

protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;

    bool startup() override;

private:
    blink::JoltPhysicsSystem* m_physicsSystem{};
};
