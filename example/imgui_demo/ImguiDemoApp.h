/*********************************************************************
 * \file   ImguiDemoApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <guis/IGuiWindow.h>

class ImguiDemoApp final : public blink::IApp
{
protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
