/**

    @file      HelloWorldApp.h
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#pragma once

#include <blink/app.h>
#include <guis/IGuiWindow.h>

class HelloWorldApp final : public blink::IApp, public IGuiWindow
{
public:
    bool initialize() override;
    void terminate() override;

    void renderUi() override;
};
