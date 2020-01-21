/*!
 * \file blink.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 *
 */
#include "blink.h"

#include <RenderModule.h>
#include <OpenGLRenderModule.h>
#include <VulkanRenderModule.h>

NS_BEGIN

App::~App()
{
}

void App::step(float dt)
{
    m_ex.systems.update_all(dt);
}

int run(const tstring& renderEngine)
{
    RenderModule* renderModule = nullptr;

    if (renderEngine == "Vulkan")
    {
        renderModule = new VulkanRenderModule();
    }
    else if (renderEngine == "OpenGL")
    {
        renderModule = new OpenGLRenderModule();
    }

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
