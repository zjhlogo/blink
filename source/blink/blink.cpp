/*!
 * \file blink.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "blink.h"

#include <blink/component/Components.h>
#include <blink/util/RenderUtil.cpp>
#include <render_vulkan/VulkanRenderModule.h>

NS_BEGIN

void IApp::update(float dt)
{
    m_world.progress(dt);
}

void IApp::render(VulkanCommandBuffer& commandBuffer)
{
    m_world.each([&](flecs::entity e, const NS::Position& pos, const NS::Rotation& rot, const NS::StaticModel& model)
                 { NS::RenderUtil::drawMesh(commandBuffer, model.mesh, model.material, pos.value, rot.value); });
}

int run(IApp& app)
{
    VulkanRenderModule renderModule;

    if (!renderModule.createDevice({1280, 720}))
    {
        renderModule.destroyDevice();
        return -1;
    }

    app.initialize(renderModule);

    while (renderModule.update())
    {
        app.update(0.016f);

        renderModule.render([&](VulkanCommandBuffer& commandBuffer) { app.render(commandBuffer); });
    }

    renderModule.waitIdle();
    app.terminate();

    renderModule.destroyDevice();

    return 0;
}

NS_END
