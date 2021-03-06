/*!
 * \file VulkanWindow.cpp
 *
 * \author zjhlogo
 * \date 2020/04/11
 *
 *
 */
#include "VulkanWindow.h"
#include "VulkanRenderModule.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

NS_BEGIN

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

VulkanWindow::VulkanWindow()
{
}

VulkanWindow::~VulkanWindow()
{
    destroy();
}

bool VulkanWindow::create(const glm::ivec2& windowSize)
{
    if (!createWindow(windowSize)) return false;

    return true;
}

void VulkanWindow::destroy()
{
    destroyWindow();
}

bool VulkanWindow::createWindow(const glm::ivec2& windowSize)
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

void VulkanWindow::destroyWindow()
{
    if (m_window != nullptr)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

NS_END
