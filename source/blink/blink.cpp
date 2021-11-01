/**

    @file      blink.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "blink.h"

#include <render_vulkan/VulkanRenderModule.h>

namespace blink
{
    int run(IApp& app)
    {
        VulkanRenderModule renderModule;

        if (renderModule.createDevice({1280, 720}))
        {
            if (app.initialize(renderModule))
            {
                while (renderModule.update())
                {
                    app.update(0.016f);

                    renderModule.render([&](VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& uniformBuffer, VulkanDescriptorPool& descriptorPool)
                                        { app.render(commandBuffer, uniformBuffer, descriptorPool); });
                }

                renderModule.waitIdle();
            }

            app.terminate();
        }

        renderModule.destroyDevice();
        return 0;
    }

} // namespace blink
