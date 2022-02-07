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
    static VkDescriptorPool g_imguiPool{};

    static void imguiCheckVkResult(VkResult err)
    {
        if (err == 0) return;
        LOGE("[vulkan] Error: VkResult = {0}", err);
        if (err < 0) abort();
    }

    static void initImgui(VulkanRenderModule& renderModule)
    {
        auto& context = renderModule.getContext();
        auto& logicalDevice = renderModule.getLogicalDevice();
        auto& swapchain = renderModule.getSwapchain();

        // 1: create descriptor pool for IMGUI
        // the size of the pool is very oversize, but it's copied from imgui demo itself.
        VkDescriptorPoolSize pool_sizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
                                             {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
                                             {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
                                             {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
                                             {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
                                             {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
                                             {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
                                             {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
                                             {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
                                             {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
                                             {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}};

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = (uint32_t)std::size(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;

        vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &g_imguiPool);

        // 2: initialize imgui library
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // this initializes imgui for SDL
        ImGui_ImplGlfw_InitForVulkan(*context.getWindow(), true);

        // this initializes imgui for Vulkan
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = context.getInstance();
        init_info.PhysicalDevice = context.getPickedPhysicalDevice();
        init_info.Device = logicalDevice;
        init_info.QueueFamily = logicalDevice.getGraphicsFamilyIndex();
        init_info.Queue = logicalDevice.getGraphicsQueue();
        init_info.DescriptorPool = g_imguiPool;
        init_info.MinImageCount = (uint32_t)swapchain.getImageCount();
        init_info.ImageCount = (uint32_t)swapchain.getImageCount();
        init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

        ImGui_ImplVulkan_Init(&init_info, swapchain.getRenderPass());

        // Upload Fonts
        {
            // Use any command queue
            auto& commandPool = renderModule.getCommandPool();
            VkCommandBuffer commandBuffer = renderModule.getCommandBuffer();

            auto err = vkResetCommandPool(logicalDevice, commandPool, 0);
            imguiCheckVkResult(err);
            VkCommandBufferBeginInfo begin_info = {};
            begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
            err = vkBeginCommandBuffer(commandBuffer, &begin_info);
            imguiCheckVkResult(err);

            ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

            VkSubmitInfo end_info = {};
            end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            end_info.commandBufferCount = 1;
            end_info.pCommandBuffers = &commandBuffer;
            err = vkEndCommandBuffer(commandBuffer);
            imguiCheckVkResult(err);
            err = vkQueueSubmit(logicalDevice.getGraphicsQueue(), 1, &end_info, VK_NULL_HANDLE);
            imguiCheckVkResult(err);

            err = vkDeviceWaitIdle(logicalDevice);
            imguiCheckVkResult(err);
            ImGui_ImplVulkan_DestroyFontUploadObjects();
        }
    }

    static void renderImgui(VulkanCommandBuffer& commandBuffer)
    {
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 3. Show another simple window.
        bool show_another_window = true;
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing
                                                                  // button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
    }

    static void termImgui(VulkanRenderModule& renderModule)
    {
        auto& logicalDevice = renderModule.getLogicalDevice();

        vkDestroyDescriptorPool(logicalDevice, g_imguiPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
    }

    int run(IApp& app)
    {
        VulkanRenderModule renderModule;

        if (renderModule.createDevice({1280, 720}))
        {
            if (ResourceMgr::getInstance().initialize(renderModule.getLogicalDevice(),
                                                      renderModule.getSwapchain(),
                                                      renderModule.getDescriptorPool(),
                                                      renderModule.getCommandPool()))
            {
                initImgui(renderModule);

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

                                renderImgui(commandBuffer);
                            });

                        if (result == VulkanRenderModule::RenderResult::Recreate)
                        {
                            renderModule.getSwapchain().recreate();
                            ResourceMgr::getInstance().recreate();
                        }
                    }

                    renderModule.waitIdle();
                }

                termImgui(renderModule);

                app.terminate();
            }

            ResourceMgr::getInstance().terminate();
        }

        renderModule.destroyDevice();
        return 0;
    }

} // namespace blink
