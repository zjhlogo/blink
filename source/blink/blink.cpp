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

#include <foundation/Log.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanCommandPool.h>
#include <render_vulkan/VulkanContext.h>
#include <render_vulkan/VulkanDescriptorPool.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanWindow.h>

namespace blink
{
    static void imguiCheckVkResult(VkResult err)
    {
        if (err == 0) return;
        LOGE("[vulkan] Error: VkResult = {0}", err);
        if (err < 0) abort();
    }

    int run(IApp& app)
    {
        VulkanRenderModule renderModule;

        if (renderModule.createDevice({1280, 720}))
        {
            //// Setup Dear ImGui context
            //IMGUI_CHECKVERSION();
            //ImGui::CreateContext();

            //// Setup Dear ImGui style
            //ImGui::StyleColorsDark();

            //// Setup Platform/Renderer backends
            //auto& context = renderModule.getContext();
            //auto& logicalDevice = renderModule.getLogicalDevice();
            //auto& swapchain = renderModule.getSwapchain();

            //ImGui_ImplGlfw_InitForVulkan(*context.getWindow(), true);
            //ImGui_ImplVulkan_InitInfo init_info = {};
            //init_info.Instance = context.getInstance();
            //init_info.PhysicalDevice = context.getPickedPhysicalDevice();
            //init_info.Device = logicalDevice;
            //init_info.QueueFamily = logicalDevice.getGraphicsFamilyIndex();
            //init_info.Queue = logicalDevice.getGraphicsQueue();
            //init_info.PipelineCache = VK_NULL_HANDLE;
            //init_info.DescriptorPool = renderModule.getDescriptorPool();
            //init_info.Subpass = 0;
            //init_info.MinImageCount = (uint32_t)swapchain.getImageCount();
            //init_info.ImageCount = (uint32_t)swapchain.getImageCount();
            //init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
            //init_info.Allocator = nullptr;
            //init_info.CheckVkResultFn = imguiCheckVkResult;
            //ImGui_ImplVulkan_Init(&init_info, swapchain.getRenderPass());

            //// Upload Fonts
            //{
            //    // Use any command queue
            //    auto& commandPool = renderModule.getCommandPool();
            //    VkCommandBuffer commandBuffer = renderModule.getCommandBuffer();

            //    auto err = vkResetCommandPool(logicalDevice, commandPool, 0);
            //    imguiCheckVkResult(err);
            //    VkCommandBufferBeginInfo begin_info = {};
            //    begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            //    begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            //    err = vkBeginCommandBuffer(commandBuffer, &begin_info);
            //    imguiCheckVkResult(err);

            //    ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

            //    VkSubmitInfo end_info = {};
            //    end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            //    end_info.commandBufferCount = 1;
            //    end_info.pCommandBuffers = &commandBuffer;
            //    err = vkEndCommandBuffer(commandBuffer);
            //    imguiCheckVkResult(err);
            //    err = vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &end_info, VK_NULL_HANDLE);
            //    imguiCheckVkResult(err);

            //    err = vkDeviceWaitIdle(logicalDevice);
            //    imguiCheckVkResult(err);
            //    ImGui_ImplVulkan_DestroyFontUploadObjects();
            //}

            if (ResourceMgr::getInstance().initialize(renderModule.getLogicalDevice(),
                                                      renderModule.getSwapchain(),
                                                      renderModule.getDescriptorPool(),
                                                      renderModule.getCommandPool()))
            {
                if (app.initialize(renderModule))
                {
                    while (renderModule.processEvent())
                    {
                        app.update(0.0f);

                        auto result = renderModule.render(
                            [&](VulkanCommandBuffer& commandBuffer, VulkanUniformBuffer& pfub, VulkanUniformBuffer& pmub, VulkanUniformBuffer& piub)
                            {
                                // record scene command
                                app.render(commandBuffer, pfub, pmub, piub);

                                //// Start the Dear ImGui frame
                                //ImGui_ImplVulkan_NewFrame();
                                //ImGui_ImplGlfw_NewFrame();
                                //ImGui::NewFrame();

                                //bool show_demo_window = true;
                                //ImGui::ShowDemoWindow(&show_demo_window);

                                //// Rendering
                                //ImGui::Render();
                                //ImDrawData* drawData = ImGui::GetDrawData();

                                //// record ui command
                                //ImGui_ImplVulkan_RenderDrawData(drawData, commandBuffer);
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
