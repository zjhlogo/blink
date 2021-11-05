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
#include <blink/resource/ResourceMgr.h>
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

    m_materialUnlit = blink::ResourceMgr::getInstance().createMaterial("resource/materials/unlit.mat");
    if (!m_materialUnlit) return false;

    m_materialWireframe = blink::ResourceMgr::getInstance().createMaterial("resource/materials/wireframe.mat");
    if (!m_materialWireframe) return false;

    addSystem(new EntityCreationSystem(m_mesh, m_materialUnlit, m_materialWireframe));
    addSystem(new RotationSystem());

    if (!initializeSystems()) return false;

    return true;
}

void HelloWorldApp::terminate()
{
    terminateSystems();

    blink::ResourceMgr::getInstance().releaseMaterial(m_materialWireframe);
    m_materialWireframe = nullptr;

    blink::ResourceMgr::getInstance().releaseMaterial(m_materialUnlit);
    m_materialUnlit = nullptr;

    SAFE_DELETE(m_mesh);
}

int main(int argc, char** argv)
{
    HelloWorldApp app;
    return blink::run(app);
}
