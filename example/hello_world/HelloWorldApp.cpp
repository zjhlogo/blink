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
#include "systems/RotationSystem.h"

#include <blink/blink.h>
#include <blink/component/Components.h>
#include <render_vulkan/VulkanRenderModule.h>

HelloWorldApp::HelloWorldApp()
{
}

HelloWorldApp::~HelloWorldApp()
{
}

bool HelloWorldApp::initialize(blink::VulkanRenderModule& renderModule)
{
    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();
    auto& commandPool = renderModule.getCommandPool();
    auto& descriptorPool = renderModule.getDescriptorPool();

    m_mesh = new blink::Mesh(logicalDevice, commandPool);
    if (!m_mesh->loadFromFile("resource/viking_room.obj")) return false;

    m_texture = new blink::VulkanTexture(logicalDevice, commandPool);
    if (!m_texture->createTexture2D("resource/viking_room.png")) return false;

    m_material = new blink::Material(logicalDevice, swapchain, descriptorPool);
    if (!m_material->create()) return false;
    m_material->setTexture(m_texture);

    addSystem(new EntityCreationSystem(m_mesh, m_material));
    addSystem(new RotationSystem());

    if (!initializeSystems()) return false;

    return true;
}

void HelloWorldApp::terminate()
{
    terminateSystems();

    SAFE_DELETE(m_material);
    SAFE_DELETE(m_texture);
    SAFE_DELETE(m_mesh);
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
