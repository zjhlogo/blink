
/*********************************************************************
 * \file   GltfViewerApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "GltfViewerApp.h"

#include <blink/blink.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>
#include <render_systems/SceneRenderSystem.h>
#include <tracy-0.8.2/Tracy.hpp>

bool GltfViewerApp::initialize()
{
    stdcpp_set_os_api();

    //m_ecsWorld.addSystem(new PbrEntityCreationSystem());
    //if (!m_ecsWorld.initialize()) return false;

    // add render systems
    addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = new ImguiRenderSystem();
    addRenderSystem(guiRenderSystem);
    if (!initializeRenderSystems()) return false;

    guiRenderSystem->addWindow(this);

    return true;
}

void GltfViewerApp::terminate()
{
    terminateRenderSystems();
    m_ecsWorld.terminate();
}

void GltfViewerApp::renderUi()
{
    //auto* entityCreation = m_ecsWorld.findSystem<PbrEntityCreationSystem>();

    //ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    //ImGui::Begin("Properties",
    //             nullptr,
    //             ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    //// material property
    //entityCreation->renderMaterialPropertyUi();

    //ImGui::End();
}

int main(int argc, char** argv)
{
    TracyMessageL("Hello");

    GltfViewerApp app;
    return blink::run(app);
}