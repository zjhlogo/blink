
/*********************************************************************
 * \file   PbrApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include "blink/app.h"
#include "common/guis/IGuiWindow.h"

class PbrApp : public blink::IApp, public IGuiWindow
{
public:
    virtual void onGui() override;

protected:
    bool startup() override;
    void shutdown() override;

    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
