#include "../../Device.h"
#include "../../../Framework.h"
#include "../../../IApp.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

GLFWwindow* s_window = nullptr;

bool Device::initialize()
{
	/* Initialize the library */
	if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	/* Create a windowed mode window and its OpenGL context */
	s_window = glfwCreateWindow(1280, 720, "BLINK", nullptr, nullptr);
	if (!s_window)
	{
		glfwTerminate();
		return false;
	}

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

int Device::start(const UpdateCb& cb)
{
    double begin = glfwGetTime();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(s_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

        double end = glfwGetTime();
		double duration = end - begin;
		begin = end;
		cb(static_cast<float>(duration));

		/* Swap front and back buffers */
		glfwSwapBuffers(s_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}
