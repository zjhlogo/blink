/*********************************************************************
 * \file   LineArtApp.h
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <common/guis/IGuiWindow.h>

class LineArtApp final : public blink::IApp, public IGuiWindow
{
public:
    void onGui() override;

protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;
};
