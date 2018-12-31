#include "../../Device.h"
#include "../../../input/MouseComponent.h"
#include "../../../Framework.h"
#include "../../../IApp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace blink
{
    static GLFWwindow* s_window = nullptr;
    static glm::ivec2 s_mousePos;

    void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
        MouseComponent* mouseComponent = Framework::getInstance().findComponent<MouseComponent>();
        if (mouseComponent)
        {
            MouseEvent evt(MouseEvent::Action::Move);
            evt.mousePos.x = static_cast<int>(xpos);
            evt.mousePos.y = static_cast<int>(ypos);
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

    bool Device::initialize(int width, int height, const char* title)
    {
        /* Initialize the library */
        if (!glfwInit()) return false;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        /* Create a windowed mode window and its OpenGL context */
        s_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!s_window)
        {
            glfwTerminate();
            return false;
        }

        glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetCursorPosCallback(s_window, mousePositionCallback);
        glfwSetMouseButtonCallback(s_window, mouseButtonCallback);

        /* Make the window's context current */
        glfwMakeContextCurrent(s_window);

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
        while (!glfwWindowShouldClose(s_window))
        {
            double end = glfwGetTime();
            double duration = end - begin;
            begin = end;

            Framework::getInstance().step(static_cast<float>(duration));

            /* Swap front and back buffers */
            glfwSwapBuffers(s_window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        return 0;
    }
}

