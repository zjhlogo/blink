/**

    @file      blink.cpp
    @brief
    @details   ~
    @author    zjhlogo
    @date      1.11.2021
    @copyright Copyright zjhlogo, 2021. All right reserved.

**/

#include "blink.h"
#include "resource/ResourceMgr.h"

#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>

namespace blink
{
    int run(IApp& app)
    {
        VulkanRenderModule renderModule;

        if (renderModule.createDevice({1280, 720}))
        {
            if (ResourceMgr::getInstance().initialize(renderModule.getLogicalDevice(), renderModule.getSwapchain()))
            {
                if (app.initialize(renderModule))
                {
                    while (renderModule.processEvent())
                    {
                        app.executeLogicalSystems();

                        auto result = renderModule.render(
                            [&](VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub)
                            {
                                //
                                app.executeRenderSystems(commandBuffer, pfub, pmub, piub);
                            });

                        if (result == VulkanRenderModule::RenderResult::Recreate)
                        {
                            renderModule.getSwapchain().recreate();
                            ResourceMgr::getInstance().recreate();
                        }
                    }

                    renderModule.waitIdle();
                }

                app.terminate();
            }

            ResourceMgr::getInstance().terminate();
        }

        renderModule.destroyDevice();
        return 0;
    }

} // namespace blink
