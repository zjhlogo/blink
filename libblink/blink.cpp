#include "blink.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace blink
{
    void App::step(double dt)
    {
        m_ex.systems.update_all(dt);
    }

    GLFWwindow* g_window = nullptr;
    static glm::ivec2 s_mousePos;

    void mousePositionCallback(GLFWwindow* window, double xpos, double ypos)
    {
    }

    void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
    {
    }

    void mouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
    }

    int run(App * app)
    {
        /* Initialize the library */
        if (!glfwInit()) return -1;

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

        /* Create a windowed mode window and its OpenGL context */
        g_window = glfwCreateWindow(1280, 720, "blink", nullptr, nullptr);
        if (!g_window)
        {
            glfwTerminate();
            return -1;
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
            return -1;
        }

        app->initialize();

        double begin = glfwGetTime();

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(g_window))
        {
            /* Poll for and process events */
            glfwPollEvents();

            double end = glfwGetTime();
            double duration = end - begin;
            begin = end;

            app->step(duration);

            /* Swap front and back buffers */
            glfwSwapBuffers(g_window);
        }

        app->terminate();
        glfwTerminate();

        return 0;
    }
}
