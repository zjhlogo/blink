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
#include <blink/components/Components.h>
#include <blink/materials/Material.h>
#include <blink/resources/ResourceMgr.h>
#include <blink/systems/DebugLineSystem.h>
#include <core/systems/AngularVelocitySystem.h>
#include <core/systems/LinearVelocitySystem.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <physics/systems/BeginPhysicsSimulationSystem.h>
#include <physics/systems/DynamicIntegrateSystem.h>
#include <physics/systems/EndPhysicsSimulationSystem.h>
#include <render_systems/ImguiRenderSystem.h>
#include <render_systems/SceneRenderSystem.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>

HelloWorldApp::HelloWorldApp()
{
    //
}

HelloWorldApp::~HelloWorldApp()
{
    //
}

bool HelloWorldApp::initialize(blink::VulkanRenderModule& renderModule)
{
    stdcpp_set_os_api();

    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();

    // add logical systems

    // physics systems
    // m_ecsWorld.addSystem(new blink::BeginPhysicsSimulationSystem());
    // m_ecsWorld.addSystem(new blink::DynamicIntegrateSystem());
    // m_ecsWorld.addSystem(new blink::EndPhysicsSimulationSystem());
    m_ecsWorld.addSystem(new SinglePendulumSystem());

    // prefab system
    m_ecsWorld.addSystem(new PrefabInitializeSystem());

    // entity creation system
    const auto& extent = swapchain.getImageExtent();
    m_ecsWorld.addSystem(new EntityCreationSystem(glm::vec2(extent.width, extent.height)));

    // add user command system
    m_ecsWorld.addSystem(new UserCommandSystem());

    m_ecsWorld.addSystem(new blink::DebugLineSystem());

    if (!m_ecsWorld.initialize()) return false;

    // add render systems
    addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = new ImguiRenderSystem(renderModule);
    addRenderSystem(guiRenderSystem);
    if (!initializeRenderSystems()) return false;

    guiRenderSystem->addWindow(this);

    return true;
}

void HelloWorldApp::terminate()
{
    terminateRenderSystems();
    m_ecsWorld.terminate();
}

void HelloWorldApp::renderUi()
{
    auto* entityCreation = m_ecsWorld.findSystem<EntityCreationSystem>();
    auto* userCommand = m_ecsWorld.findSystem<UserCommandSystem>();

    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties",
                 nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

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
