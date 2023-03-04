/**

    @file      PendulumApp.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/app.h>
#include <common/guis/IGuiWindow.h>

class PendulumApp final : public blink::IApp, public IGuiWindow
{
public:
    void onGui() override;

protected:
    bool initializeLogicalSystems() override;
    bool initializeRenderSystems() override;

};
