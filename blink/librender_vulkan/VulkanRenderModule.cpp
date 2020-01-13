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
#include <chrono>
#include <set>

NS_BEGIN

struct Vertex
{
    glm::vec2 pos;
    glm::vec3 color;

    static vk::VertexInputBindingDescription getBindingDescription()
    {
        vk::VertexInputBindingDescription bindingDescription;
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex);
        bindingDescription.inputRate = vk::VertexInputRate::eVertex;
        return bindingDescription;
    }

    static std::array<vk::VertexInputAttributeDescription, 2> getAttributeDescriptions()
    {
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions;

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = vk::Format::eR32G32Sfloat;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = vk::Format::eR32G32B32Sfloat;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;
    }
};

const std::vector<Vertex> g_vertices = { { { -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
                                         { { 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } },
                                         { { 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f } },
                                         { { -0.5f, 0.5f }, { 1.0f, 1.0f, 1.0f } } };

const std::vector<uint16_t> g_indices = { 0, 1, 2, 0, 2, 3 };

struct UniformBufferObject
{
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
};

static void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
    VulkanRenderModule* renderModule = reinterpret_cast<VulkanRenderModule*>(glfwGetWindowUserPointer(window));
    renderModule->setFrameBBufferResized(true);
}

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

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
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
    if (!createSwapChain()) return false;
    if (!createImageViews()) return false;
    if (!createRenderPass()) return false;
    if (!createDescriptorSetLayout()) return false;
    if (!createGraphicsPipeline()) return false;
    if (!createFramebuffers()) return false;
    if (!createCommandPool()) return false;
    if (!createVertexBuffer()) return false;
    if (!createIndexBuffer()) return false;
    if (!createUniformBuffers()) return false;
    if (!createDescriptorPool()) return false;
    if (!createDescriptorSets()) return false;
    if (!createCommandBuffers()) return false;
    if (!createSyncObjects()) return false;

    return true;
}

void VulkanRenderModule::destroyDevice()
{
    cleanSwapChain();

    destroyDescriptorSetLayout();
    destroyUniformBuffers();
    destroyDescriptorSets();
    destroyDescriptorPool();
    destroyIndexBuffer();
    destroyVertexBuffer();
    destroySyncObjects();
    destroyCommandPool();
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

    // app->step(static_cast<float>(duration));

    drawFrame();

    m_logicalDevice.waitIdle();

    return true;
}

void VulkanRenderModule::drawFrame()
{
    m_logicalDevice.waitForFences(m_inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    uint32_t imageIndex{};
    vk::Result result = m_logicalDevice.acquireNextImageKHR(m_swapChain,
                                                            std::numeric_limits<uint64_t>::max(),
                                                            m_imageAvailableSemaphores[m_currentFrame],
                                                            vk::Fence(),
                                                            &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
        recreateSwapChain();
        return;
    }
    else if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    updateUniformBuffer(imageIndex);

    if (m_imagesInFlight[imageIndex])
    {
        m_logicalDevice.waitForFences(m_imagesInFlight[imageIndex], VK_TRUE, std::numeric_limits<uint64_t>::max());
    }
    m_imagesInFlight[imageIndex] = m_inFlightFences[m_currentFrame];

    vk::SubmitInfo submitInfo;
    vk::Semaphore waitSemaphores[] = { m_imageAvailableSemaphores[m_currentFrame] };
    vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];

    vk::Semaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    m_logicalDevice.resetFences(m_inFlightFences[m_currentFrame]);
    m_graphicsQueue.submit(submitInfo, m_inFlightFences[m_currentFrame]);

    vk::PresentInfoKHR presentInfo;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    vk::SwapchainKHR swapChains[] = { m_swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = m_presentQueue.presentKHR(&presentInfo);
    if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR || m_frameBufferResized)
    {
        m_frameBufferResized = false;
        recreateSwapChain();
    }
    else if (result != vk::Result::eSuccess)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    m_currentFrame = (m_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

bool VulkanRenderModule::createWindow(const glm::ivec2& windowSize)
{
    m_deviceSize = windowSize;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    /* Create a windowed mode window and its OpenGL context */
    m_window = glfwCreateWindow(windowSize.x, windowSize.y, "blink", nullptr, nullptr);
    if (!m_window) return false;

    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, frameBufferResizeCallback);

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
    createInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                                 | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    createInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                             | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
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

bool VulkanRenderModule::createSwapChain()
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
        int width, height;
        glfwGetFramebufferSize(m_window, &width, &height);
        vk::Extent2D actualExtent(width, height);
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

void VulkanRenderModule::destroySwapChain()
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

bool VulkanRenderModule::createDescriptorSetLayout()
{
    vk::DescriptorSetLayoutBinding uboLayoutBinding;
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = vk::DescriptorType::eUniformBuffer;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = vk::ShaderStageFlagBits::eVertex;
    uboLayoutBinding.pImmutableSamplers = nullptr;

    vk::DescriptorSetLayoutCreateInfo layoutInfo;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;
    m_descriptorSetLayout = m_logicalDevice.createDescriptorSetLayout(layoutInfo);

    return true;
}

void VulkanRenderModule::destroyDescriptorSetLayout()
{
    m_logicalDevice.destroyDescriptorSetLayout(m_descriptorSetLayout);
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

    // vertex input state
    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // input assembly state
    vk::PipelineInputAssemblyStateCreateInfo inputAssembly;
    inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

    // viewport state
    vk::Viewport viewport(0.0f,
                          0.0f,
                          static_cast<float>(m_swapChainExtent.width),
                          static_cast<float>(m_swapChainExtent.height),
                          0.0f,
                          1.0f);
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
    colorBlendAttachment.colorWriteMask =
        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
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
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &m_descriptorSetLayout;
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

bool VulkanRenderModule::createVertexBuffer()
{
    vk::DeviceSize bufferSize = sizeof(g_vertices[0]) * g_vertices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;

    createBuffer(stagingBuffer,
                 stagingBufferMemory,
                 bufferSize,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    void* data = m_logicalDevice.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, g_vertices.data(), static_cast<size_t>(bufferSize));
    m_logicalDevice.unmapMemory(stagingBufferMemory);

    createBuffer(m_vertexBuffer,
                 m_vertexBufferMemory,
                 bufferSize,
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eVertexBuffer,
                 vk::MemoryPropertyFlagBits::eDeviceLocal);

    copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

    m_logicalDevice.destroyBuffer(stagingBuffer);
    m_logicalDevice.freeMemory(stagingBufferMemory);

    return true;
}

void VulkanRenderModule::destroyVertexBuffer()
{
    m_logicalDevice.destroyBuffer(m_vertexBuffer);
    m_logicalDevice.freeMemory(m_vertexBufferMemory);
}

bool VulkanRenderModule::createIndexBuffer()
{
    vk::DeviceSize bufferSize = sizeof(g_indices[0]) * g_indices.size();

    vk::Buffer stagingBuffer;
    vk::DeviceMemory stagingBufferMemory;
    createBuffer(stagingBuffer,
                 stagingBufferMemory,
                 bufferSize,
                 vk::BufferUsageFlagBits::eTransferSrc,
                 vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

    void* data = m_logicalDevice.mapMemory(stagingBufferMemory, 0, bufferSize);
    memcpy(data, g_indices.data(), static_cast<size_t>(bufferSize));
    m_logicalDevice.unmapMemory(stagingBufferMemory);

    createBuffer(m_indexBuffer,
                 m_indexBufferMemory,
                 bufferSize,
                 vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eIndexBuffer,
                 vk::MemoryPropertyFlagBits::eDeviceLocal);
    copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

    m_logicalDevice.destroyBuffer(stagingBuffer);
    m_logicalDevice.freeMemory(stagingBufferMemory);

    return true;
}

void VulkanRenderModule::destroyIndexBuffer()
{
    m_logicalDevice.destroyBuffer(m_indexBuffer);
    m_logicalDevice.freeMemory(m_indexBufferMemory);
}

bool VulkanRenderModule::createUniformBuffers()
{
    vk::DeviceSize bufferSize = sizeof(UniformBufferObject);
    m_uniformBuffers.resize(m_swapChainImages.size());
    m_uniformBuffersMemory.resize(m_swapChainImages.size());

    for (size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        createBuffer(m_uniformBuffers[i],
                     m_uniformBuffersMemory[i],
                     bufferSize,
                     vk::BufferUsageFlagBits::eUniformBuffer,
                     vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);
    }

    return true;
}

void VulkanRenderModule::destroyUniformBuffers()
{
    for (size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        m_logicalDevice.destroyBuffer(m_uniformBuffers[i]);
        m_logicalDevice.freeMemory(m_uniformBuffersMemory[i]);
    }
}

bool VulkanRenderModule::createDescriptorPool()
{
    vk::DescriptorPoolSize poolSize;
    poolSize.type = vk::DescriptorType::eUniformBuffer;
    poolSize.descriptorCount = static_cast<uint32_t>(m_swapChainImages.size());

    vk::DescriptorPoolCreateInfo poolInfo;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(m_swapChainImages.size());
    m_descriptorPool = m_logicalDevice.createDescriptorPool(poolInfo);

    return true;
}

void VulkanRenderModule::destroyDescriptorPool()
{
    m_logicalDevice.destroyDescriptorPool(m_descriptorPool);
}

bool VulkanRenderModule::createDescriptorSets()
{
    std::vector<vk::DescriptorSetLayout> layouts(m_swapChainImages.size(), m_descriptorSetLayout);

    vk::DescriptorSetAllocateInfo allocInfo;
    allocInfo.descriptorPool = m_descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(m_swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();
    m_descriptorSets.resize(m_swapChainImages.size());
    m_descriptorSets = m_logicalDevice.allocateDescriptorSets(allocInfo);

    for (size_t i = 0; i < m_swapChainImages.size(); ++i)
    {
        vk::DescriptorBufferInfo bufferInfo;
        bufferInfo.buffer = m_uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        vk::WriteDescriptorSet descriptorWrite;
        descriptorWrite.dstSet = m_descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;
        descriptorWrite.pTexelBufferView = nullptr;
        m_logicalDevice.updateDescriptorSets(1, &descriptorWrite, 0, nullptr);
    }

    return true;
}

void VulkanRenderModule::destroyDescriptorSets()
{
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
                vk::DeviceSize offset = 0;
                m_commandBuffers[i].bindVertexBuffers(0, m_vertexBuffer, offset);
                m_commandBuffers[i].bindIndexBuffer(m_indexBuffer, 0, vk::IndexType::eUint16);
                m_commandBuffers[i]
                    .bindDescriptorSets(vk::PipelineBindPoint::eGraphics, m_pipelineLayout, 0, 1, &m_descriptorSets[i], 0, nullptr);
                m_commandBuffers[i].drawIndexed(static_cast<uint32_t>(g_indices.size()), 1, 0, 0, 0);
            }

            m_commandBuffers[i].endRenderPass();
        }

        m_commandBuffers[i].end();
    }

    return true;
}

void VulkanRenderModule::destroyCommandBuffers()
{
    m_logicalDevice.freeCommandBuffers(m_commandPool, m_commandBuffers);
}

bool VulkanRenderModule::createSyncObjects()
{
    m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    m_imagesInFlight.resize(m_swapChainImages.size());

    vk::SemaphoreCreateInfo semaphoreInfo;
    vk::FenceCreateInfo fenceInfo;
    fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        m_imageAvailableSemaphores[i] = m_logicalDevice.createSemaphore(semaphoreInfo);
        m_renderFinishedSemaphores[i] = m_logicalDevice.createSemaphore(semaphoreInfo);
        m_inFlightFences[i] = m_logicalDevice.createFence(fenceInfo);
    }

    return true;
}

void VulkanRenderModule::destroySyncObjects()
{
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
    {
        m_logicalDevice.destroySemaphore(m_renderFinishedSemaphores[i]);
        m_logicalDevice.destroySemaphore(m_imageAvailableSemaphores[i]);
        m_logicalDevice.destroyFence(m_inFlightFences[i]);
    }
}

bool VulkanRenderModule::recreateSwapChain()
{
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(m_window, &width, &height);

    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(m_window, &width, &height);
        glfwWaitEvents();
    }

    m_logicalDevice.waitIdle();

    cleanSwapChain();

    createSwapChain();
    createImageViews();
    createRenderPass();
    createGraphicsPipeline();
    createFramebuffers();
    createUniformBuffers();
    createDescriptorPool();
    createDescriptorSets();
    createCommandBuffers();

    return true;
}

void VulkanRenderModule::cleanSwapChain()
{
    destroyFramebuffers();
    destroyCommandBuffers();
    destroyGraphicsPipeline();
    destroyRenderPass();
    destroyImageViews();
    destroySwapChain();
}

const std::vector<const char*>& VulkanRenderModule::getRequiredValidationLayers()
{
    static const std::vector<const char*> REQUIRED_LAYERS = { "VK_LAYER_KHRONOS_validation" };

    return REQUIRED_LAYERS;
}

bool VulkanRenderModule::checkValidationLayerSupported(const std::vector<vk::LayerProperties>& layers,
                                                       const std::vector<const char*>& requiredLayers)
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
    static const std::vector<const char*> REQUIRED_EXTENSIONS = {
        "VK_KHR_surface",
        "VK_KHR_win32_surface",
        "VK_EXT_debug_utils",
    };

    return REQUIRED_EXTENSIONS;
}

const std::vector<const char*>& VulkanRenderModule::getRequiredDeviceExtensions()
{
    static const std::vector<const char*> REQUIRED_EXTENSIONS = {
        "VK_KHR_swapchain",
    };

    return REQUIRED_EXTENSIONS;
}

bool VulkanRenderModule::checkExtensionsSupported(const std::vector<vk::ExtensionProperties>& extensions,
                                                  const std::vector<const char*>& requiredExtensions)
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

        if (graphicsFamilyIndex != -1 && presentFamilyIndex != -1 && extensionsSupported && swapChainAdequate)
        {
            return index;
        }

        ++index;
    }

    return -1;
}

bool VulkanRenderModule::getBestFitQueueFamilyPropertyIndex(int& graphicsFamily,
                                                            int& presentFamily,
                                                            const vk::PhysicalDevice& physicalDevice,
                                                            const vk::SurfaceKHR& surface,
                                                            const std::vector<vk::QueueFamilyProperties>& queueFamilies)
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

uint32_t VulkanRenderModule::findMemoryType(uint32_t typeFilter, vk::MemoryPropertyFlags properties)
{
    vk::PhysicalDeviceMemoryProperties memProperties = m_physicalDevice.getMemoryProperties();
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
    {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) return i;
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

bool VulkanRenderModule::createBuffer(vk::Buffer& buffer,
                                      vk::DeviceMemory& bufferMemory,
                                      vk::DeviceSize size,
                                      vk::BufferUsageFlags usage,
                                      vk::MemoryPropertyFlags properties)
{
    vk::BufferCreateInfo bufferInfo;
    bufferInfo.size = sizeof(g_vertices[0]) * g_vertices.size();
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = vk::SharingMode::eExclusive;
    buffer = m_logicalDevice.createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = m_logicalDevice.getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
    bufferMemory = m_logicalDevice.allocateMemory(allocInfo);
    m_logicalDevice.bindBufferMemory(buffer, bufferMemory, 0);

    return true;
}

void VulkanRenderModule::copyBuffer(vk::Buffer& srcBuffer, vk::Buffer& dstBuffer, vk::DeviceSize& size)
{
    vk::CommandBufferAllocateInfo allocInfo;
    allocInfo.level = vk::CommandBufferLevel::ePrimary;
    allocInfo.commandPool = m_commandPool;
    allocInfo.commandBufferCount = 1;

    std::vector<vk::CommandBuffer> commandBuffers = m_logicalDevice.allocateCommandBuffers(allocInfo);
    auto& commandBuffer = commandBuffers[0];

    vk::CommandBufferBeginInfo beginInfo;
    beginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
    commandBuffer.begin(beginInfo);

    vk::BufferCopy copyRegion;
    copyRegion.srcOffset = 0;
    copyRegion.dstOffset = 0;
    copyRegion.size = size;
    commandBuffer.copyBuffer(srcBuffer, dstBuffer, copyRegion);

    commandBuffer.end();

    vk::SubmitInfo submitInfo;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    m_graphicsQueue.submit(submitInfo, vk::Fence());
    m_graphicsQueue.waitIdle();

    m_logicalDevice.freeCommandBuffers(m_commandPool, commandBuffer);
}

void VulkanRenderModule::updateUniformBuffer(uint32_t currentImage)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo;
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), m_swapChainExtent.width / (float)m_swapChainExtent.height, 0.1f, 10.0f);

    void* data = m_logicalDevice.mapMemory(m_uniformBuffersMemory[currentImage], 0, sizeof(ubo));
    memcpy(data, &ubo, sizeof(ubo));
    m_logicalDevice.unmapMemory(m_uniformBuffersMemory[currentImage]);
}

NS_END
