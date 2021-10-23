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

App::~App()
{
}

void App::step(float dt)
{
    m_world.progress(dt);
}

int run(const tstring& renderEngine)
{
    VulkanRenderModule* renderModule = new VulkanRenderModule();

    if (!renderModule) return -1;

    if (!renderModule->createDevice({ 1280, 720 }))
    {
        renderModule->destroyDevice();
        SAFE_DELETE(renderModule);
        return -1;
    }

    while (renderModule->gameLoop())
    {
        // TODO: sleep ?
    }

    renderModule->destroyDevice();
    SAFE_DELETE(renderModule);

    return 0;
}

NS_END
