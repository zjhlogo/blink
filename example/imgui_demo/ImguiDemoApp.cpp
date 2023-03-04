/*********************************************************************
 * \file   ImguiDemoApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "ImguiDemoApp.h"
#include "panels/ControlPanel.h"
#include "panels/DetailPanel.h"
#include "panels/MainMenuBar.h"

#include <blink/blink.h>
#include <common/render_systems/ImguiRenderSystem.h>

bool ImguiDemoApp::initializeLogicalSystems()
{
    return true;
}

bool ImguiDemoApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(new MainMenuBar());
    guiRenderSystem->addWindow(new ControlPanel());
    guiRenderSystem->addWindow(new DetailPanel());

    return true;
}

int main(int argc, char** argv)
{
    ImguiDemoApp app;
    return blink::run(argc, argv, app);
}
