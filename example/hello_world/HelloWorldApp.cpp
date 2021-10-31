/**

    @file      HelloWorldApp.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      31.10.2021
    @copyright © zjhlogo, 2021. All right reserved.

**/
#include "HelloWorldApp.h"

#include <blink/blink.h>
#include <blink/component/Components.h>
#include <render_vulkan/VulkanRenderModule.h>

HelloWorldApp::HelloWorldApp()
{
}

HelloWorldApp::~HelloWorldApp()
{
}

bool HelloWorldApp::initialize(NS::VulkanRenderModule& renderModule)
{
    auto& logicalDevice = renderModule.getLogicalDevice();
    auto& swapchain = renderModule.getSwapchain();
    auto& commandPool = renderModule.getCommandPool();
    auto& descriptorPool = renderModule.getDescriptorPool();

    m_mesh = new NS::Mesh(logicalDevice, commandPool);
    if (!m_mesh->loadFromFile("resource/viking_room.obj")) return false;

    m_material = new NS::Material(logicalDevice, swapchain, commandPool, descriptorPool);
    if (!m_material->create()) return false;

    auto e1 = m_world.entity();
    e1.set<NS::Position>({glm::zero<glm::vec3>()});
    e1.set<NS::Rotation>({glm::identity<glm::quat>()});
    e1.set<NS::StaticModel>({m_mesh, m_material});

    //auto e2 = m_world.entity();
    //e2.set<NS::Position>({ glm::vec3(0.5f, 0.0f, 0.0f) });
    //e2.set<NS::Rotation>({ glm::identity<glm::quat>() });
    //e2.set<NS::StaticModel>({ m_mesh, m_material });

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_DELETE(m_material);
    SAFE_DELETE(m_mesh);
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return NS::run(app);
}
