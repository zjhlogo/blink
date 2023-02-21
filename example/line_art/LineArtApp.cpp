/*********************************************************************
 * \file   LineArtApp.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   04/24/2022
 *********************************************************************/
#include "LineArtApp.h"
#include "systems/LineArtEntityCreationSystem.h"

#include <blink/blink.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>

bool LineArtApp::initializeLogicalSystems()
{
    addLogicSystem(new LineArtEntityCreationSystem());

    return true;
}

bool LineArtApp::initializeRenderSystems()
{
    auto guiRenderSystem = addRenderSystem(new ImguiRenderSystem());
    guiRenderSystem->addWindow(this);

    return true;
}

void LineArtApp::onGui()
{
    const auto& ecsWorld = getEcsWorld();
    auto* entityCreation = ecsWorld.findSystem<LineArtEntityCreationSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // material property
    entityCreation->renderMaterialPropertyUi();

    ImGui::End();
}

int main(int argc, char** argv)
{
    LineArtApp app;
    return blink::run(argc, argv, app);
}
