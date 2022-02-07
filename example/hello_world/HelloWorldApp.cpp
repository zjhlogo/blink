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
    addLogicalSystem(new EntityCreationSystem(glm::vec2(extent.width, extent.height)));
    if (!initializeLogicalSystems()) return false;

    // add render systems
    addRenderSystem(new SceneRenderSystem(this));
    auto guiRenderSystem = new ImguiRenderSystem(renderModule);
    addRenderSystem(guiRenderSystem);
    if (!initializeRenderSystems()) return false;

    guiRenderSystem->addWindow(this);
    m_pbrMaterial = blink::ResourceMgr::getInstance().createMaterial("resource/materials/pbr_lit.mtl");

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_RELEASE(m_pbrMaterial);
    terminateRenderSystems();
    terminateLogicalSystems();
}

void HelloWorldApp::renderUi()
{
    static float s_roughness = 0.1f;
    static float s_metallic = 1.0f;
    static glm::vec3 s_color = {1.0f, 1.0f, 1.0f};

    ImGui::Begin("Pbr Properties Window");

    if (ImGui::SliderFloat("roughness", &s_roughness, 0.0f, 1.0f, "roughness = %.3f"))
    {
        m_pbrMaterial->setRoughness(s_roughness);
    }
    if (ImGui::SliderFloat("metallic", &s_metallic, 0.0f, 1.0f, "metallic = %.3f"))
    {
        m_pbrMaterial->setMetallic(s_metallic);
    }
    if (ImGui::ColorPicker3("color", (float*)&s_color))
    {
        m_pbrMaterial->setColor(s_color);
    }

    ImGui::End();
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
