/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "VulkanRenderModule.h"
#include <File.h>
#include <set>

namespace blink
{
    static void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
    }

    static void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
    }

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            // TODO: Message is important enough to show
        }

        tstring strError = pCallbackData->pMessage;
        return VK_FALSE;
    }

    VulkanRenderModule::VulkanRenderModule()
    {

    }

    VulkanRenderModule::~VulkanRenderModule()
    {

    }

    bool VulkanRenderModule::createDevice(const glm::ivec2& deviceSize)
    {
        /* Initialize the library */
        if (!glfwInit()) return false;

        if (!createWindow(deviceSize)) return false;
        if (!createInstance()) return false;
        if (!setupDebugMessenger()) return false;
        if (!createSurface()) return false;
        if (!pickPhysicalDevice()) return false;
        if (!createLogicalDevice()) return false;
        if (!createSwapchain()) return false;
        if (!createImageViews()) return false;
        if (!createRenderPass()) return false;
        if (!createGraphicsPipeline()) return false;
        if (!createFramebuffers()) return false;
        if (!createCommandPool()) return false;
        if (!createCommandBuffers()) return false;
        if (!createSemaphores()) return false;

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
        m_logicalDevice.waitIdle();

        destroySemaphores();
        destroyCommandBuffers();
        destroyCommandPool();
        destroyFramebuffers();
        destroyGraphicsPipeline();
        destroyRenderPass();
        destroyImageViews();
        destroySwapchain();
        destroyLogicalDevice();
        destroySurface();
        destroyDebugMessenger();
        destroyInstance();
        destroyWindow();
        glfwTerminate();
    }

    bool VulkanRenderModule::gameLoop()
    {
        double begin = glfwGetTime();

        /* Loop until the user closes the window */
        if (glfwWindowShouldClose(m_window)) return false;

        /* Poll for and process events */
        glfwPollEvents();

        double end = glfwGetTime();
        double duration = end - begin;
        begin = end;

//         app->step(static_cast<float>(duration));
        
        drawFrame();

        return true;
    }

    void VulkanRenderModule::drawFrame()
    {
        uint32_t imageIndex{};
        m_logicalDevice.acquireNextImageKHR(m_swapChain, std::numeric_limits<uint64_t>::max(), m_imageAvailableSemaphore, vk::Fence(), &imageIndex);

        vk::SubmitInfo submitInfo;
        vk::Semaphore waitSemaphores[] = { m_imageAvailableSemaphore };
        vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

        vk::Semaphore signalSemaphores[] = { m_renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        m_graphicsQueue.submit(1, &submitInfo, vk::Fence());

        vk::PresentInfoKHR presentInfo;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        vk::SwapchainKHR swapChains[] = { m_swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;

        m_presentQueue.presentKHR(presentInfo);
    }

    bool VulkanRenderModule::createWindow(const glm::ivec2& windowSize)
    {
        m_deviceSize = windowSize;

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(windowSize.x, windowSize.y, "blink", nullptr, nullptr);
        if (!m_window) return false;

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(m_window, mousePositionCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, mouseScrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        return true;
    }

    void VulkanRenderModule::destroyWindow()
    {
        glfwDestroyWindow(m_window);
    }

    bool VulkanRenderModule::createInstance()
    {
        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = "vulkan";
        appInfo.pEngineName = "blink";
        appInfo.apiVersion = VK_API_VERSION_1_0;

        vk::InstanceCreateInfo createInfo;
        createInfo.pApplicationInfo = &appInfo;

        // this is the vulkan supported layers
        std::vector<vk::LayerProperties> layers = vk::enumerateInstanceLayerProperties();
        if (checkValidationLayerSupported(layers, getRequiredValidationLayers()))
        {
            const auto& requiredLayers = getRequiredValidationLayers();
            createInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
            createInfo.ppEnabledLayerNames = requiredLayers.data();
        }

        // this is the vulkan supported extensions
        std::vector<vk::ExtensionProperties> extensions = vk::enumerateInstanceExtensionProperties();
        if (checkExtensionsSupported(extensions, getRequiredInstanceExtensions()))
        {
            const auto& requiredExtensions = getRequiredInstanceExtensions();
            createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            createInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }

        m_instance = vk::createInstance(createInfo);
        m_dispatchLoader.init(m_instance);

        return true;
    }

    void VulkanRenderModule::destroyInstance()
    {
        m_instance.destroy();
    }

    bool VulkanRenderModule::setupDebugMessenger()
    {
        vk::DebugUtilsMessengerCreateInfoEXT createInfo;
        createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
        createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
        createInfo.pfnUserCallback = debugCallback;
        m_debugMessenger = m_instance.createDebugUtilsMessengerEXT(createInfo, nullptr, m_dispatchLoader);

        return true;
    }

    void VulkanRenderModule::destroyDebugMessenger()
    {
        m_instance.destroyDebugUtilsMessengerEXT(m_debugMessenger, nullptr, m_dispatchLoader);
    }

    bool VulkanRenderModule::createSurface()
    {
        VkSurfaceKHR surface;
        if (VK_SUCCESS != glfwCreateWindowSurface((VkInstance)m_instance, m_window, nullptr, &surface))
        {
            return false;
        }
        m_surface = vk::SurfaceKHR(surface);

        return true;
    }

    void VulkanRenderModule::destroySurface()
    {
        m_instance.destroySurfaceKHR(m_surface);
    }

    bool VulkanRenderModule::pickPhysicalDevice()
    {
        std::vector<vk::PhysicalDevice> physicalDevices = m_instance.enumeratePhysicalDevices();
        int index = getBestFitPhysicalDeviceIndex(physicalDevices, m_surface);
        if (index != -1)
        {
            m_physicalDevice = physicalDevices[index];
            return true;
        }

        return false;
    }

    bool VulkanRenderModule::createLogicalDevice()
    {
        // get best fit queue index from queue families
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        if (!getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, m_physicalDevice, m_surface, queueFamilyProperties))
        {
            return false;
        }

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        std::set<int> uniqueQueueFamilies = { graphicsFamilyIndex, presentFamilyIndex };

        float priority = 1.0f;
        for (uint32_t queueFamilyIndex : uniqueQueueFamilies)
        {
            vk::DeviceQueueCreateInfo queueCreateInfo;
            queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &priority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::PhysicalDeviceFeatures deviceFeatures;

        vk::DeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
        deviceCreateInfo.enabledExtensionCount = 0;

        std::vector<vk::LayerProperties> layers = m_physicalDevice.enumerateDeviceLayerProperties();
        if (checkValidationLayerSupported(layers, getRequiredValidationLayers()))
        {
            const auto& requiredLayers = getRequiredValidationLayers();
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = requiredLayers.data();
        }

        std::vector<vk::ExtensionProperties> extensions = m_physicalDevice.enumerateDeviceExtensionProperties();
        if (checkExtensionsSupported(extensions, getRequiredDeviceExtensions()))
        {
            const auto& requiredExtensions = getRequiredDeviceExtensions();
            deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
            deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
        }

        m_logicalDevice = m_physicalDevice.createDevice(deviceCreateInfo);

        // retriveing queue handles
        m_graphicsQueue = m_logicalDevice.getQueue(graphicsFamilyIndex, 0);
        m_presentQueue = m_logicalDevice.getQueue(presentFamilyIndex, 0);

        return true;
    }

    void VulkanRenderModule::destroyLogicalDevice()
    {
        m_logicalDevice.destroy();
    }

    bool VulkanRenderModule::createSwapchain()
    {
        // select format
        std::vector<vk::SurfaceFormatKHR> formats = m_physicalDevice.getSurfaceFormatsKHR(m_surface);
        vk::SurfaceFormatKHR selFormat = formats[0];
        for (const auto& format : formats)
        {
            if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            {
                selFormat = format;
                break;
            }
        }

        // select present mode
        std::vector<vk::PresentModeKHR> presentModes = m_physicalDevice.getSurfacePresentModesKHR(m_surface);
        vk::PresentModeKHR selPresentMode = vk::PresentModeKHR::eFifo;
        for (const auto& presentMode : presentModes)
        {
            if (presentMode == vk::PresentModeKHR::eMailbox)
            {
                selPresentMode = presentMode;
                break;
            }
            else if (presentMode == vk::PresentModeKHR::eImmediate)
            {
                selPresentMode = presentMode;
            }
        }

        // select extent
        vk::SurfaceCapabilitiesKHR capabilities = m_physicalDevice.getSurfaceCapabilitiesKHR(m_surface);
        vk::Extent2D selExtent;
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            selExtent = capabilities.currentExtent;
        }
        else
        {
            vk::Extent2D actualExtent(m_deviceSize.x, m_deviceSize.y);
            actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
            selExtent = actualExtent;
        }

        // select image count
        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) imageCount = capabilities.maxImageCount;

        vk::SwapchainCreateInfoKHR createInfo;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = selFormat.format;
        createInfo.imageColorSpace = selFormat.colorSpace;
        createInfo.imageExtent = selExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;

        // get best fit queue index from queue families
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, m_physicalDevice, m_surface, queueFamilyProperties);
        if (graphicsFamilyIndex != presentFamilyIndex)
        {
            uint32_t queueFamilyIndexs[2] = { static_cast<uint32_t>(graphicsFamilyIndex), static_cast<uint32_t>(presentFamilyIndex) };
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndexs;
        }
        else
        {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = capabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = selPresentMode;
        createInfo.clipped = VK_TRUE;

        m_swapChain = m_logicalDevice.createSwapchainKHR(createInfo);
        m_swapChainImages = m_logicalDevice.getSwapchainImagesKHR(m_swapChain);

        m_swapChainImageFormat = selFormat.format;
        m_swapChainExtent = selExtent;

        return true;
    }

    void VulkanRenderModule::destroySwapchain()
    {
        m_logicalDevice.destroySwapchainKHR(m_swapChain);
    }

    bool VulkanRenderModule::createImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
        {
            vk::ImageViewCreateInfo createInfo;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = vk::ImageViewType::e2D;
            createInfo.format = m_swapChainImageFormat;
            createInfo.subresourceRange.aspectMask = vk::ImageAspectFlagBits::eColor;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            m_swapChainImageViews[i] = m_logicalDevice.createImageView(createInfo);
        }

        return true;
    }

    void VulkanRenderModule::destroyImageViews()
    {
        for (auto& imageView : m_swapChainImageViews)
        {
            m_logicalDevice.destroyImageView(imageView);
        }
        m_swapChainImageViews.clear();
    }

    bool VulkanRenderModule::createRenderPass()
    {
        // color attachment
        vk::AttachmentDescription colorAttachment;
        colorAttachment.format = m_swapChainImageFormat;
        colorAttachment.samples = vk::SampleCountFlagBits::e1;
        colorAttachment.loadOp = vk::AttachmentLoadOp::eClear;
        colorAttachment.storeOp = vk::AttachmentStoreOp::eStore;
        colorAttachment.stencilLoadOp = vk::AttachmentLoadOp::eDontCare;
        colorAttachment.stencilStoreOp = vk::AttachmentStoreOp::eDontCare;
        colorAttachment.initialLayout = vk::ImageLayout::eUndefined;
        colorAttachment.finalLayout = vk::ImageLayout::ePresentSrcKHR;

        // subpass
        vk::AttachmentReference colorAttachmentRef;
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = vk::ImageLayout::eColorAttachmentOptimal;

        vk::SubpassDescription subpass;
        subpass.pipelineBindPoint = vk::PipelineBindPoint::eGraphics;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        // create render pass
        vk::RenderPassCreateInfo renderPassInfo;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;

        vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, 0);
        dependency.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
        dependency.dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead | vk::AccessFlagBits::eColorAttachmentWrite;

        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        m_renderPass = m_logicalDevice.createRenderPass(renderPassInfo);

        return true;
    }

    void VulkanRenderModule::destroyRenderPass()
    {
        m_logicalDevice.destroyRenderPass(m_renderPass);
    }

    bool VulkanRenderModule::createGraphicsPipeline()
    {
        std::vector<uint8> vertShaderCode;
        readFileIntoBuffer(vertShaderCode, "resource/shaders/shader_base.vert.spv");

        std::vector<uint8> fragShaderCode;
        readFileIntoBuffer(fragShaderCode, "resource/shaders/shader_base.frag.spv");

        vk::ShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        vk::ShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        vk::PipelineShaderStageCreateInfo vertShaderStageInfo;
        vertShaderStageInfo.stage = vk::ShaderStageFlagBits::eVertex;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        vk::PipelineShaderStageCreateInfo fragShaderStageInfo;
        fragShaderStageInfo.stage = vk::ShaderStageFlagBits::eFragment;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        // shader state
        vk::PipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        // TODO: do it later
        // vertex input state
        vk::PipelineVertexInputStateCreateInfo vertexInputInfo;

        // input assembly state
        vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

        // viewport state
        vk::Viewport viewport(0.0f, 0.0f, static_cast<float>(m_swapChainExtent.width), static_cast<float>(m_swapChainExtent.height), 0.0f, 1.0f);
        vk::Rect2D sissor({ 0, 0 }, m_swapChainExtent);
        vk::PipelineViewportStateCreateInfo viewportState;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &sissor;

        // rasterizer state
        vk::PipelineRasterizationStateCreateInfo rasterizer;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = vk::PolygonMode::eFill;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = vk::CullModeFlagBits::eBack;
        rasterizer.frontFace = vk::FrontFace::eClockwise;
        rasterizer.depthBiasEnable = VK_FALSE;

        // multisampling state
        vk::PipelineMultisampleStateCreateInfo multisampling;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = vk::SampleCountFlagBits::e1;

        // depth and stencil state
        // vk::PipelineDepthStencilStateCreateInfo;

        // color blending state
        vk::PipelineColorBlendAttachmentState colorBlendAttachment;
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
        colorBlendAttachment.blendEnable = VK_FALSE;

        vk::PipelineColorBlendStateCreateInfo colorBlending;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = vk::LogicOp::eCopy;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        // dynamic state
//         VkDynamicState dynamicStates[] = {
//             VK_DYNAMIC_STATE_VIEWPORT,
//             VK_DYNAMIC_STATE_LINE_WIDTH
//         };
// 
//         VkPipelineDynamicStateCreateInfo dynamicState = {};
//         dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//         dynamicState.dynamicStateCount = 2;
//         dynamicState.pDynamicStates = dynamicStates;

        // layout state
        vk::PipelineLayoutCreateInfo pipelineLayoutInfo;
        m_pipelineLayout = m_logicalDevice.createPipelineLayout(pipelineLayoutInfo);

        // create pipeline
        vk::GraphicsPipelineCreateInfo pipelineInfo;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
//        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlending;
//        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.subpass = 0;

        m_pipeline = m_logicalDevice.createGraphicsPipeline(vk::PipelineCache(), pipelineInfo);

        m_logicalDevice.destroyShaderModule(fragShaderModule);
        m_logicalDevice.destroyShaderModule(vertShaderModule);

        return true;
    }

    void VulkanRenderModule::destroyGraphicsPipeline()
    {
        m_logicalDevice.destroyPipeline(m_pipeline);
        m_logicalDevice.destroyPipelineLayout(m_pipelineLayout);
    }

    bool VulkanRenderModule::createFramebuffers()
    {
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); ++i)
        {
            vk::FramebufferCreateInfo frameBufferInfo;
            frameBufferInfo.renderPass = m_renderPass;
            frameBufferInfo.attachmentCount = 1;
            frameBufferInfo.pAttachments = &m_swapChainImageViews[i];
            frameBufferInfo.width = m_swapChainExtent.width;
            frameBufferInfo.height = m_swapChainExtent.height;
            frameBufferInfo.layers = 1;

            m_swapChainFramebuffers[i] = m_logicalDevice.createFramebuffer(frameBufferInfo);
        }

        return true;
    }

    void VulkanRenderModule::destroyFramebuffers()
    {
        for (auto framebuffer : m_swapChainFramebuffers)
        {
            m_logicalDevice.destroyFramebuffer(framebuffer);
        }
        m_swapChainFramebuffers.clear();
    }

    bool VulkanRenderModule::createCommandPool()
    {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties = m_physicalDevice.getQueueFamilyProperties();

        int graphicsFamilyIndex{};
        int presentFamilyIndex{};
        getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, m_physicalDevice, m_surface, queueFamilyProperties);

        vk::CommandPoolCreateInfo poolInfo;
        poolInfo.queueFamilyIndex = static_cast<uint32_t>(graphicsFamilyIndex);

        m_commandPool = m_logicalDevice.createCommandPool(poolInfo);

        return true;
    }

    void VulkanRenderModule::destroyCommandPool()
    {
        m_logicalDevice.destroyCommandPool(m_commandPool);
    }

    bool VulkanRenderModule::createCommandBuffers()
    {
        vk::CommandBufferAllocateInfo allocInfo;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_swapChainFramebuffers.size());
        m_commandBuffers = m_logicalDevice.allocateCommandBuffers(allocInfo);

        for (size_t i = 0; i < m_commandBuffers.size(); ++i)
        {
            vk::CommandBufferBeginInfo beginInfo;
            beginInfo.flags = vk::CommandBufferUsageFlagBits::eSimultaneousUse;

            m_commandBuffers[i].begin(beginInfo);

            {
                vk::RenderPassBeginInfo renderPassInfo;
                renderPassInfo.renderPass = m_renderPass;
                renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
                renderPassInfo.renderArea.offset = { 0, 0 };
                renderPassInfo.renderArea.extent = m_swapChainExtent;

                vk::ClearValue clearColor;
                vk::ClearColorValue colorValue;
                colorValue.setFloat32({ 0.0f, 0.0f, 0.0f, 1.0f });
                clearColor.color = colorValue;

                renderPassInfo.clearValueCount = 1;
                renderPassInfo.pClearValues = &clearColor;

                m_commandBuffers[i].beginRenderPass(renderPassInfo, vk::SubpassContents::eInline);

                {
                    m_commandBuffers[i].bindPipeline(vk::PipelineBindPoint::eGraphics, m_pipeline);
                    m_commandBuffers[i].draw(3, 1, 0, 0);
                }

                m_commandBuffers[i].endRenderPass();
            }

            m_commandBuffers[i].end();
        }

        return true;
    }

    void VulkanRenderModule::destroyCommandBuffers()
    {

    }

    bool VulkanRenderModule::createSemaphores()
    {
        vk::SemaphoreCreateInfo semaphoreInfo;
        m_imageAvailableSemaphore = m_logicalDevice.createSemaphore(semaphoreInfo);
        m_renderFinishedSemaphore = m_logicalDevice.createSemaphore(semaphoreInfo);

        return true;
    }

    void VulkanRenderModule::destroySemaphores()
    {
        m_logicalDevice.destroySemaphore(m_renderFinishedSemaphore);
        m_logicalDevice.destroySemaphore(m_imageAvailableSemaphore);
    }

    const std::vector<const char*>& VulkanRenderModule::getRequiredValidationLayers()
    {
        static const std::vector<const char*> REQUIRED_LAYERS =
        {
            "VK_LAYER_KHRONOS_validation"
        };

        return REQUIRED_LAYERS;
    }

    bool VulkanRenderModule::checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers, const std::vector<const char *>& requiredLayers)
    {
        for (const auto& requireLayer : requiredLayers)
        {
            bool layerFound = false;
            for (const auto& layer : layers)
            {
                if (std::strcmp(requireLayer, layer.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) return false;
        }

        return true;
    }

    const std::vector<const char*>& VulkanRenderModule::getRequiredInstanceExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS =
        {
            "VK_KHR_surface",
            "VK_KHR_win32_surface",
            "VK_EXT_debug_utils",
        };

        return REQUIRED_EXTENSIONS;
    }

    const std::vector<const char*>& VulkanRenderModule::getRequiredDeviceExtensions()
    {
        static const std::vector<const char*> REQUIRED_EXTENSIONS =
        {
            "VK_KHR_swapchain",
        };

        return REQUIRED_EXTENSIONS;
    }

    bool VulkanRenderModule::checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions, const std::vector<const char *>& requiredExtensions)
    {
        for (const auto& requiredExtension : requiredExtensions)
        {
            bool extensionFound = false;
            for (const auto& extension : extensions)
            {
                if (std::strcmp(requiredExtension, extension.extensionName) == 0)
                {
                    extensionFound = true;
                    break;
                }
            }

            if (!extensionFound) return false;
        }

        return true;
    }

    int VulkanRenderModule::getBestFitPhysicalDeviceIndex(const std::vector<vk::PhysicalDevice>& physicalDevices, const vk::SurfaceKHR& surface)
    {
        int index = 0;
        for (const auto& device : physicalDevices)
        {
            std::vector<vk::QueueFamilyProperties> queueFamilyProperties = device.getQueueFamilyProperties();

            int graphicsFamilyIndex{};
            int presentFamilyIndex{};
            getBestFitQueueFamilyPropertyIndex(graphicsFamilyIndex, presentFamilyIndex, device, surface, queueFamilyProperties);

            auto extensions = device.enumerateDeviceExtensionProperties();
            bool extensionsSupported = checkExtensionsSupported(extensions, getRequiredDeviceExtensions());

            bool swapChainAdequate = false;
            if (extensionsSupported)
            {
                vk::SurfaceCapabilitiesKHR capabilities = device.getSurfaceCapabilitiesKHR(surface);
                std::vector<vk::SurfaceFormatKHR> formats = device.getSurfaceFormatsKHR(surface);
                std::vector<vk::PresentModeKHR> presentModes = device.getSurfacePresentModesKHR(surface);

                swapChainAdequate = !formats.empty() && !presentModes.empty();
            }

            if (graphicsFamilyIndex != -1
                && presentFamilyIndex != -1
                && extensionsSupported
                && swapChainAdequate)
            {
                return index;
            }

            ++index;
        }

        return -1;
    }

    bool VulkanRenderModule::getBestFitQueueFamilyPropertyIndex(int& graphicsFamily, int& presentFamily, const vk::PhysicalDevice& physicalDevice, const vk::SurfaceKHR& surface, const std::vector<vk::QueueFamilyProperties>& queueFamilies)
    {
        graphicsFamily = -1;
        presentFamily = -1;

        uint32_t i = 0;
        for (const auto& queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics)
            {
                graphicsFamily = i;
            }

            if (queueFamily.queueCount > 0 && physicalDevice.getSurfaceSupportKHR(i, surface))
            {
                presentFamily = i;
            }

            if (graphicsFamily != -1 && presentFamily != -1)
            {
                return true;
            }
        }

        return false;
    }

    vk::ShaderModule VulkanRenderModule::createShaderModule(const std::vector<uint8>& shaderCode)
    {
        vk::ShaderModuleCreateInfo createInfo;
        createInfo.codeSize = shaderCode.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderCode.data());

        return m_logicalDevice.createShaderModule(createInfo);
    }

    bool VulkanRenderModule::readFileIntoBuffer(std::vector<uint8>& bufferOut, const std::string& filePath)
    {
        File file(filePath);
        file.read(bufferOut, file.fileSize());
        file.close();

        return true;
    }

}
