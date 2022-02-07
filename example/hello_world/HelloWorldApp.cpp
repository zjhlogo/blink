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

#include <blink/blink.h>
#include <blink/component/Components.h>
#include <blink/material/Material.h>
#include <blink/resource/ResourceMgr.h>
#include <blink/system/AngularVelocitySystem.h>
#include <blink/system/LinearVelocitySystem.h>
#include <imgui/imgui.h>
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
    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();

    // add logical systems
    // m_world.set_threads(4);
    addLogicalSystem(new blink::LinearVelocitySystem());
    addLogicalSystem(new blink::AngularVelocitySystem());

    const auto& extent = swapchain.getImageExtent();
    m_sys = new EntityCreationSystem(glm::vec2(extent.width, extent.height));
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
    static float s_roughness = 0.1f;
    static float s_metallic = 1.0f;
    static glm::vec3 s_surfaceColor = {1.0f, 1.0f, 1.0f};

    static glm::vec3 s_lightColor = {1.0f, 1.0f, 1.0f};
    static float s_intensity = 100.0f;

    ImGui::Begin("Pbr Properties Window");

    if (ImGui::SliderFloat("roughness", &s_roughness, 0.0f, 1.0f, "roughness = %.3f"))
    {
        auto material = m_sys->m_sphere.get<blink::StaticModel>()->material;
        material->setRoughness(s_roughness);
    }
    if (ImGui::SliderFloat("metallic", &s_metallic, 0.0f, 1.0f, "metallic = %.3f"))
    {
        auto material = m_sys->m_sphere.get<blink::StaticModel>()->material;
        material->setMetallic(s_metallic);
    }
    if (ImGui::ColorEdit3("surface color", (float*)&s_surfaceColor))
    {
        auto material = m_sys->m_sphere.get<blink::StaticModel>()->material;
        material->setColor(s_surfaceColor);
    }

    if (ImGui::ColorEdit3("light color", (float*)&s_lightColor))
    {
        const auto lightData = m_sys->m_light.get<blink::LightData>();
        m_sys->m_light.set(blink::LightData{s_lightColor, lightData->intensity});
    }
    if (ImGui::SliderFloat("light intensity", &s_intensity, 0.0f, 1000.0f, "intensity = %.3f"))
    {
        const auto lightData = m_sys->m_light.get<blink::LightData>();
        m_sys->m_light.set(blink::LightData{lightData->color, s_intensity});
    }

    ImGui::End();
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
