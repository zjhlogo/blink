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
    return true;
}

void HelloWorldApp::terminate()
{
    terminateRenderSystems();
    terminateLogicalSystems();
}

void HelloWorldApp::renderUi()
{
    // 3. Show another simple window.
    bool show_another_window = true;
    if (show_another_window)
    {
        // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) show_another_window = false;
        ImGui::End();
    }
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
