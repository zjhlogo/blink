/*!
 * \file blink.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "blink.h"

#include <render_vulkan/VulkanRenderModule.h>

NS_BEGIN

IApp::~IApp()
{
}

void IApp::update(float dt)
{
    m_world.progress(dt);
}

void IApp::render(VulkanCommandBuffer& commandBuffer)
{

}

void recordCommands(IApp* app, VulkanCommandBuffer& commandBuffer)
{
    app->render(commandBuffer);
}

int run(IApp& app)
{
    VulkanRenderModule renderModule;

    if (!renderModule.createDevice({ 1280, 720 }))
    {
        renderModule.destroyDevice();
        return -1;
    }

    app.initialize(renderModule);

    while (renderModule.update())
    {
        app.update(0.016f);

        renderModule.render(std::bind(recordCommands, &app, std::placeholders::_1));
    }

    renderModule.waitIdle();
    app.terminate();

    renderModule.destroyDevice();

    return 0;
}

NS_END
