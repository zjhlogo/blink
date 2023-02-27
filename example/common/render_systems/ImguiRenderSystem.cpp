/*********************************************************************
 * \file   ImguiRenderSystem.cpp
 * \brief
 *
 * \author zjhlogo
 * \date   02/07/2022
 *********************************************************************/

#include "ImguiRenderSystem.h"
#include "../utils/ImguiExtension.h"

#include <foundation/Log.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_vulkan.h>
#include <imgui/imgui.h>
#include <render_vulkan/VulkanCommandBuffer.h>
#include <render_vulkan/VulkanContext.h>
#include <render_vulkan/VulkanLogicalDevice.h>
#include <render_vulkan/VulkanRenderModule.h>
#include <render_vulkan/VulkanRenderPass.h>
#include <render_vulkan/VulkanSwapChain.h>
#include <render_vulkan/VulkanWindow.h>

#include <algorithm>

static void imguiCheckVkResult(VkResult err)
{
    if (err == 0)
    {
        return;
    }
    LOGE("[vulkan] Error: VkResult = {0}", err);
    if (err < 0)
    {
        abort();
    }
}

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
    auto& renderPass = renderModule->getRenderPass();
    auto& swapchain = renderModule->getSwapChain();

    // Create Descriptor Pool
    {
        VkDescriptorPoolSize pool_sizes[] = {
            {               VK_DESCRIPTOR_TYPE_SAMPLER, 1000},
            {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
            {         VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000},
            {         VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000},
            {  VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000},
            {  VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000},
            {        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
            {        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000},
            {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000},
            {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000},
            {      VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000}
        };
        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
        pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
        pool_info.pPoolSizes = pool_sizes;
        vkCreateDescriptorPool((VkDevice)logicalDevice, &pool_info, nullptr, &m_imguiPool);
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
    io.ConfigFlags |= ImGuiCol_DockingEmptyBg;        //
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    //  io.ConfigViewportsNoAutoMerge = true;
    //  io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)*context.getWindow(), true);

    // this initializes imgui for Vulkan
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = context.getInstance();
    init_info.PhysicalDevice = context.getPickedPhysicalDevice();
    init_info.Device = (VkDevice)logicalDevice;
    init_info.QueueFamily = logicalDevice.getGraphicsFamilyIndex();
    init_info.Queue = logicalDevice.getGraphicsQueue();
    init_info.PipelineCache = VK_NULL_HANDLE;
    init_info.DescriptorPool = m_imguiPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = static_cast<uint32_t>(swapchain.getImageCount());
    init_info.ImageCount = static_cast<uint32_t>(swapchain.getImageCount());
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = VK_NULL_HANDLE;
    init_info.CheckVkResultFn = imguiCheckVkResult;
    ImGui_ImplVulkan_Init(&init_info, (VkRenderPass)renderPass);

    // Upload Fonts
    {
        // Use any command queue
        logicalDevice.executeCommand([](const blink::VulkanCommandBuffer& commandBuffer) {
            //
            ImGui_ImplVulkan_CreateFontsTexture((VkCommandBuffer)commandBuffer);
        });
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }

    return true;
}

void ImguiRenderSystem::terminate()
{
    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());
    auto& logicalDevice = renderModule->getLogicalDevice();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    vkDestroyDescriptorPool((VkDevice)logicalDevice, m_imguiPool, nullptr);
}

void ImguiRenderSystem::render()
{
    if (m_allWindows.empty())
    {
        return;
    }

    auto renderModule = dynamic_cast<blink::VulkanRenderModule*>(blink::getRenderModule());

    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
    ImGuiIO& io = ImGui::GetIO();
    ImGuiExt::lastFrameMouseWheel = io.MouseWheel;
    ImGuiExt::lastFrameMouseWheelH = io.MouseWheelH;

    for (auto window : m_allWindows)
    {
        window->onGui();
    }

    // Rendering
    ImGui::Render();

    ImDrawData* main_draw_data = ImGui::GetDrawData();
    const bool main_is_minimized = (main_draw_data->DisplaySize.x <= 0.0f || main_draw_data->DisplaySize.y <= 0.0f);

    if (!main_is_minimized)
    {
        ImGui_ImplVulkan_RenderDrawData(main_draw_data, (VkCommandBuffer)renderModule->getCommandBuffer());
    }

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImguiRenderSystem::removeWindow(const IGuiWindow* window)
{
    auto it = std::find(m_allWindows.begin(), m_allWindows.end(), window);
    if (it != m_allWindows.end())
    {
        m_allWindows.erase(it);
    }
}
