/*********************************************************************
 * \file   LineArtApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <guis/IGuiWindow.h>

class LineArtApp final : public blink::IApp, public IGuiWindow
{
public:
    LineArtApp() = default;
    ~LineArtApp() override = default;

    bool initialize() override;
    void terminate() override;

    void renderUi() override;
};
