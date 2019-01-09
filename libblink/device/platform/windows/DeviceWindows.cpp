#include "../../Device.h"
#include "../../../input/MouseComponent.h"
#include "../../../Framework.h"
#include "../../../IApp.h"
#include <Log.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace blink
{
    GLFWwindow* g_window = nullptr;
    static glm::ivec2 s_mousePos;

    void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        MouseComponent* mouseComponent = Framework::getInstance().findComponent<MouseComponent>();
        if (mouseComponent)
        {
            MouseEvent evt(MouseEvent::Action::Move);
            evt.mousePos.x = static_cast<float>(xpos);
            evt.mousePos.y = static_cast<float>(ypos);
            mouseComponent->postEvent(evt);
        }
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
        MouseComponent* mouseComponent = Framework::getInstance().findComponent<MouseComponent>();
        if (mouseComponent)
        {
            MouseEvent::Action act = MouseEvent::Action::ButtonDown;
            if (action == GLFW_PRESS) act = MouseEvent::Action::ButtonDown;
            else if (action == GLFW_RELEASE) act = MouseEvent::Action::ButtonUp;

            MouseEvent evt(act);
            evt.mouseButton = static_cast<MouseEvent::MouseButton>(button);
            evt.modifyKey = mods;
            mouseComponent->postEvent(evt);
        }
    }

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        MouseComponent* mouseComponent = Framework::getInstance().findComponent<MouseComponent>();
        if (mouseComponent)
        {
            MouseEvent evt(MouseEvent::Action::Scroll);
            evt.mouseScroll = { static_cast<float>(xoffset), static_cast<float>(yoffset) };
            mouseComponent->postEvent(evt);
        }
    }

    bool Device::initialize(int width, int height, const char* title)
    {
        /* Initialize the library */
        if (!glfwInit()) return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        /* Create a windowed mode window and its OpenGL context */
        g_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!g_window)
        {
            glfwTerminate();
            return false;
        }

        glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(g_window, mousePositionCallback);
        glfwSetMouseButtonCallback(g_window, mouseButtonCallback);
        glfwSetScrollCallback(g_window, mouseScrollCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(g_window);

        // access gl API after context created
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwTerminate();
            return false;
        }

        return true;
    }

    void Device::terminate()
    {
        glfwTerminate();
    }

    int Device::start()
    {
        double begin = glfwGetTime();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(g_window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            double end = glfwGetTime();
            double duration = end - begin;
            begin = end;

            Framework::getInstance().step(static_cast<float>(duration));

            /* Swap front and back buffers */
            glfwSwapBuffers(g_window);
        }

        return 0;
    }
}
