/*!
 * \file VulkanRenderModule.cpp
 *
 * \author zjhlogo
 * \date 2019/07/29
 *
 * 
 */
#include "VulkanRenderModule.h"

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

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(deviceSize.x, deviceSize.y, "blink", nullptr, nullptr);
        if (!m_window) return false;

        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(m_window, mousePositionCallback);
        glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
        glfwSetScrollCallback(m_window, mouseScrollCallback);
        glfwSetKeyCallback(m_window, keyCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(m_window);

        VkSurfaceKHR surface;
        VkResult err = glfwCreateWindowSurface(instance, window, NULL, &surface);

        return true;
    }

    void VulkanRenderModule::destroyDevice()
    {
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

        /* Swap front and back buffers */
        glfwSwapBuffers(m_window);

        return true;
    }

}
