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

#include <blink/blink.h>
#include <blink/component/Components.h>
#include <blink/material/Material.h>
#include <blink/resource/ResourceMgr.h>
#include <core/system/AngularVelocitySystem.h>
#include <core/system/LinearVelocitySystem.h>
#include <flecs/flecs_os_api_stdcpp.h>
#include <imgui/imgui.h>
#include <physics/system/BeginPhysicsSimulationSystem.h>
#include <physics/system/DynamicIntegrateSystem.h>
#include <physics/system/EndPhysicsSimulationSystem.h>
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
    addLogicalSystem(new blink::BeginPhysicsSimulationSystem());
    addLogicalSystem(new blink::DynamicIntegrateSystem());
    addLogicalSystem(new blink::EndPhysicsSimulationSystem());

    auto prefabSystem = new PrefabInitializeSystem();
    addLogicalSystem(prefabSystem);
    const auto& extent = swapchain.getImageExtent();
    m_sys = new EntityCreationSystem(prefabSystem, glm::vec2(extent.width, extent.height));
    addLogicalSystem(m_sys);
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
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f), 0, ImVec2(1.0f, 0.0f));
    ImGui::Begin("Properties", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    // light property
    {
        if (ImGui::CollapsingHeader("light property", ImGuiTreeNodeFlags_DefaultOpen))
        {
            blink::LightData lightData = *m_sys->m_light.get<blink::LightData>();
            if (ImGui::ColorEdit3("color1", (float*)&lightData.color))
            {
                m_sys->m_light.set<blink::LightData>(lightData);
            }
            if (ImGui::SliderFloat("intensity", &lightData.intensity, 0.0f, 1000.0f, "intensity = %.3f"))
            {
                m_sys->m_light.set<blink::LightData>(lightData);
            }
        }
    }

    // material property
    {
        if (ImGui::CollapsingHeader("material property", ImGuiTreeNodeFlags_DefaultOpen))
        {
            auto material = m_sys->m_sphere.get<blink::StaticModel>()->material;
            auto roughness = material->getRoughness();
            if (ImGui::SliderFloat("roughness", &roughness, 0.0f, 1.0f, "roughness = %.3f"))
            {
                material->setRoughness(roughness);
            }
            auto metallic = material->getMetallic();
            if (ImGui::SliderFloat("metallic", &metallic, 0.0f, 1.0f, "metallic = %.3f"))
            {
                material->setMetallic(metallic);
            }
            glm::vec3 color = material->getColor();
            if (ImGui::ColorEdit3("color2", (float*)&color))
            {
                material->setColor(color);
            }
        }
    }

    ImGui::End();
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
