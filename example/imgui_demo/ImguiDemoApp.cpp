/*********************************************************************
 * \file   ImguiDemoApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   February 2023
 *********************************************************************/
#include "ImguiDemoApp.h"

#include <blink/blink.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>

bool ImguiDemoApp::initializeLogicalSystems()
{
    return true;
}

bool ImguiDemoApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);

    return true;
}

void ImguiDemoApp::renderUi()
{
    if (ImGui::Begin("ControlPanel"))
    {
        ImGui::End();
    }
}

int main(int argc, char** argv)
{
    ImguiDemoApp app;
    return blink::run(app);
}
