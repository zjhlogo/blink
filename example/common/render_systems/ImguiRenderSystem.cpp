/*********************************************************************
 * \file   ImguiRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "ImguiRenderSystem.h"

#include <foundation/Log.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanContext.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanSwapchain.h>
#include <render_vulkan/VulkanWindow.h>

#include <algorithm>

// static void imguiCheckVkResult(VkResult err)
// {
//     if (err == 0) return;
//     LOGE("[vulkan] Error: VkResult = {0}", err);
//     if (err < 0) abort();
// }

ImguiRenderSystem::~ImguiRenderSystem()
{
    //
    m_allWindows.clear();
}

bool ImguiRenderSystem::initialize()
{
    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());
    auto& context = renderModule->getContext();
    auto& logicalDevice = renderModule->getLogicalDevice();
    auto& swapchain = renderModule->getSwapchain();

    // 1: create descriptor pool for IMGUI
    // the size of the pool is very oversize, but it's copied from imgui demo itself.
    VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
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
    pool_info.poolSizeCount = static_cast<uint32_t>(std::size(poolSizes));
    pool_info.pPoolSizes = poolSizes;

    vkCreateDescriptorPool(logicalDevice, &pool_info, nullptr, &m_imguiPool);

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
    init_info.DescriptorPool = m_imguiPool;
    init_info.MinImageCount = static_cast<uint32_t>(swapchain.getImageCount());
    init_info.ImageCount = static_cast<uint32_t>(swapchain.getImageCount());
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;

    ImGui_ImplVulkan_Init(&init_info, swapchain.getRenderPass());

    // Upload Fonts
    {
        // Use any command queue
        logicalDevice.executeCommand(
            [](const blink::VulkanCommandBuffer& commandBuffer)
            {
                //
                ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
            });
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    return true;
}

void ImguiRenderSystem::terminate()
{
    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());
    auto& logicalDevice = renderModule->getLogicalDevice();

    vkDestroyDescriptorPool(logicalDevice, m_imguiPool, nullptr);
    ImGui_ImplVulkan_Shutdown();
}

void ImguiRenderSystem::render()
{
    if (m_allWindows.empty()) return;

    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    for (auto window : m_allWindows)
    {
        window->renderUi();
    }

    // Rendering
    ImGui::Render();

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), renderModule->getCommandBuffer());
}

void ImguiRenderSystem::addWindow(IGuiWindow* window)
{
    m_allWindows.push_back(window);
}

void ImguiRenderSystem::removeWindow(const IGuiWindow* window)
{
    auto it = std::find(m_allWindows.begin(), m_allWindows.end(), window);
    if (it != m_allWindows.end())
    {
        m_allWindows.erase(it);
    }
}
