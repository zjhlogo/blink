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
#include <core/modules/IRenderModule.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>
#include <render_systems/SceneRenderSystem.h>
#include <tracy-0.8.2/Tracy.hpp>

bool PbrApp::initialize()
{
    stdcpp_set_os_api();

    m_ecsWorld.addSystem(new PbrEntityCreationSystem());
    if (!m_ecsWorld.initialize()) return false;

    // add render systems
    auto renderModule = blink::getRenderModule();
    renderModule->addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = dynamic_cast<ImguiRenderSystem*>(renderModule->addRenderSystem(new ImguiRenderSystem()));
    guiRenderSystem->addWindow(this);

    return true;
}

void PbrApp::terminate()
{
    m_ecsWorld.terminate();
}

void PbrApp::renderUi()
{
    auto* entityCreation = m_ecsWorld.findSystem<PbrEntityCreationSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // material property
    entityCreation->renderMaterialPropertyUi();

    ImGui::End();
}

int main(int argc, char** argv)
{
    TracyMessageL("Hello");

    PbrApp app;
    return blink::run(app);
}
