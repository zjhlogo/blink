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
    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();
    auto& commandPool = renderModule.getCommandPool();
    auto& descriptorPool = renderModule.getDescriptorPool();

    //m_world.set_threads(4);

    addSystem(new blink::LinearVelocitySystem());
    addSystem(new blink::AngularVelocitySystem());

    const auto& extent = swapchain.getImageExtent();
    addSystem(new EntityCreationSystem(glm::vec2(extent.width, extent.height)));

    if (!initializeSystems()) return false;

    return true;
}

void HelloWorldApp::terminate()
{
    terminateSystems();
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
