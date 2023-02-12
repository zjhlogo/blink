/*********************************************************************
 * \file   PbrApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "PbrApp.h"
#include "systems/PbrEntityCreationSystem.h"

#include <blink/blink.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>

bool PbrApp::initializeLogicalSystems()
{
    addLogicSystem(new PbrEntityCreationSystem());
    return true;
}

bool PbrApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);
    return true;
}

void PbrApp::onGui()
{
    const auto& ecsWorld = getEcsWorld();
    auto* entityCreation = ecsWorld.findSystem<PbrEntityCreationSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // material property
    entityCreation->renderMaterialPropertyUi();

    ImGui::End();
}

int main(int argc, char** argv)
{
    PbrApp app;
    return blink::run(app);
}
