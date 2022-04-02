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
#include "systems/UserCommandSystem.h"

#include <blink/blink.h>
#include <blink/components/Components.h>
#include <blink/materials/Material.h>
#include <blink/resources/ResourceMgr.h>
#include <core/systems/AngularVelocitySystem.h>
#include <core/systems/LinearVelocitySystem.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <physics/systems/BeginPhysicsSimulationSystem.h>
#include <physics/systems/DynamicIntegrateSystem.h>
#include <physics/systems/EndPhysicsSimulationSystem.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <systems/ImguiRenderSystem.h>
#include <systems/SceneRenderSystem.h>

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
    m_world.set_threads(4);
    m_world.set_target_fps(60.0f);

    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();

    // add logical systems

    // physics systems
    addLogicalSystem(new blink::BeginPhysicsSimulationSystem());
    addLogicalSystem(new blink::DynamicIntegrateSystem());
    addLogicalSystem(new blink::EndPhysicsSimulationSystem());

    // prefab system
    auto prefabSystem = new PrefabInitializeSystem();
    addLogicalSystem(prefabSystem);

    // entity creation system
    const auto& extent = swapchain.getImageExtent();
    addLogicalSystem(new EntityCreationSystem(prefabSystem, glm::vec2(extent.width, extent.height)));

    // add user command system
    addLogicalSystem(new UserCommandSystem());

    if (!initializeLogicalSystems()) return false;

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
    terminateLogicalSystems();
}

void HelloWorldApp::renderUi()
{
    auto* entityCreation = getLogicalSystem<EntityCreationSystem>();
    auto* userCommand = getLogicalSystem<UserCommandSystem>();

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
