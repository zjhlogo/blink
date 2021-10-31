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

int run(IApp& app)
{
    VulkanRenderModule renderModule;

    if (renderModule.createDevice({1280, 720}))
    {
        if (app.initialize(renderModule))
        {
            if (app.postInitialize())
            {
                while (renderModule.update())
                {
                    app.update(0.016f);

                    renderModule.render([&](VulkanCommandBuffer& commandBuffer) { app.render(commandBuffer); });
                }

                renderModule.waitIdle();
            }

            app.preTerminate();
        }

        app.terminate();
    }

    renderModule.destroyDevice();
    return 0;
}

NS_END
