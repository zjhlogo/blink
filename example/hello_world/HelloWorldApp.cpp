/*!
 * \file HelloWorldApp.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "HelloWorldApp.h"

#include <blink/util/RenderUtil.h>
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

    return true;
}

void HelloWorldApp::terminate()
{
    SAFE_DELETE(m_material);
    SAFE_DELETE(m_mesh);
}

void HelloWorldApp::update(float dt)
{
    m_material->updateUniformBuffer();
}

void HelloWorldApp::render(NS::VulkanCommandBuffer& commandBuffer)
{
    NS::RenderUtil::drawMesh(commandBuffer, m_mesh, m_material, glm::vec3(), glm::quat());
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return NS::run(app);
}
