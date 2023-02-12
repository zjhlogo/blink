/**

    @file      HelloWorldApp.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/
#include "HelloWorldApp.h"
#include "systems/EntityCreationSystem.h"
#include "systems/PrefabInitializeSystem.h"
#include "systems/SinglePendulumSystem.h"
#include "systems/UserCommandSystem.h"

#include <blink/blink.h>
#include <blink/systems/DebugLineSystem.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <render_systems/ImguiRenderSystem.h>
#include <render_systems/SceneRenderSystem.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>

bool HelloWorldApp::initialize()
{
    stdcpp_set_os_api();

    // add logical systems
    // physics systems
    // m_ecsWorld.addSystem(new blink::BeginPhysicsSimulationSystem());
    // m_ecsWorld.addSystem(new blink::DynamicIntegrateSystem());
    // m_ecsWorld.addSystem(new blink::EndPhysicsSimulationSystem());

    m_ecsWorld.addSystem(new SinglePendulumSystem());
    m_ecsWorld.addSystem(new PrefabInitializeSystem());
    m_ecsWorld.addSystem(new EntityCreationSystem());
    m_ecsWorld.addSystem(new UserCommandSystem());
    m_ecsWorld.addSystem(new blink::DebugLineSystem());
    if (!m_ecsWorld.initialize()) return false;

    // add render systems
    auto renderModule = blink::getRenderModule();
    renderModule->addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = dynamic_cast<ImguiRenderSystem*>(renderModule->addRenderSystem(new ImguiRenderSystem()));
    guiRenderSystem->addWindow(this);

    return true;
}

void HelloWorldApp::terminate()
{
    m_ecsWorld.terminate();
}

void HelloWorldApp::renderUi()
{
    auto* entityCreation = m_ecsWorld.findSystem<EntityCreationSystem>();
    auto* userCommand = m_ecsWorld.findSystem<UserCommandSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // light property
    entityCreation->renderLightPropertyUi();

    // material property
    entityCreation->renderMaterialPropertyUi();

    // user input
    userCommand->renderUi();

    ImGui::End();
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
