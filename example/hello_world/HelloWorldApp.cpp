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
#include <blink/geometry/builder/BoxBuilder.h>
#include <blink/geometry/builder/MeshBuilder.h>
#include <blink/geometry/builder/PlaneBuilder.h>
#include <blink/geometry/builder/SphereUvBuilder.h>
#include <blink/geometry/builder/TetrahedronBuilder.h>
#include <blink/material/Material.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanTexture.h>

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

    //{
    //    blink::PlaneBuilder builder;
    //    m_mesh = builder.createGeometry(logicalDevice, commandPool);
    //}

    //{
    //    blink::BoxBuilder builder;
    //    m_mesh = builder.createGeometry(logicalDevice, commandPool);
    //}

    //{
    //    blink::SphereUvBuilder builder;
    //    m_mesh = builder.createGeometry(logicalDevice, commandPool);
    //}

    {
        blink::TetrahedronBuilder builder;
        m_mesh = builder.createGeometry(logicalDevice, commandPool);
    }

    //{
    //    blink::MeshBuilder builder;
    //    m_mesh = builder.filePath("resource/monkey.gltf").createGeometry(logicalDevice, commandPool);
    //}

    m_texture = new blink::VulkanTexture(logicalDevice, commandPool);
    if (!m_texture->createTexture2D("resource/grid.png")) return false;

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
