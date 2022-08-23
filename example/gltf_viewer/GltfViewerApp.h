
/*********************************************************************
 * \file   GltfViewerApp.h
 * \brief  
 * 
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#pragma once

#include <blink/app.h>
#include <guis/IGuiWindow.h>

class GltfViewerApp : public blink::IApp, public IGuiWindow
{
public:
    GltfViewerApp() = default;
    virtual ~GltfViewerApp() = default;

    virtual bool initialize() override;
    virtual void terminate() override;

    virtual void renderUi() override;

};
