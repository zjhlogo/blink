
/*********************************************************************
 * \file   PbrApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <guis/IGuiWindow.h>

class PbrApp : public blink::IApp, public IGuiWindow
{
public:
    virtual void renderUi() override;

protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
