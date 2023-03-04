/**

    @file      PendulumApp.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "PendulumApp.h"
#include "systems/EntityCreationSystem.h"
#include "systems/PrefabInitializeSystem.h"
#include "systems/SinglePendulumSystem.h"
#include "systems/UserCommandSystem.h"

#include <blink/blink.h>
#include <imgui/imgui.h>
#include <common/render_systems/ImguiRenderSystem.h>

bool PendulumApp::initializeLogicalSystems()
{
    addLogicSystem(new SinglePendulumSystem());
    addLogicSystem(new PrefabInitializeSystem());
    addLogicSystem(new EntityCreationSystem());
    addLogicSystem(new UserCommandSystem());

    return true;
}

bool PendulumApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);

    return true;
}

void PendulumApp::onGui()
{
    const auto& ecsWorld = getEcsWorld();
    auto* entityCreation = ecsWorld.findSystem<EntityCreationSystem>();
    auto* userCommand = ecsWorld.findSystem<UserCommandSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // light property
    entityCreation->renderLightPropertyUi();

    // material property
    entityCreation->renderMaterialPropertyUi();

    // user input
    userCommand->onGui();

    ImGui::End();
}

int main(int argc, char** argv)
{
    PendulumApp app;
    return blink::run(argc, argv, app);
}
