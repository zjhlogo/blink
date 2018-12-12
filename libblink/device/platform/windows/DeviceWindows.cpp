#include "../../Device.h"
#include "../../../Framework.h"
#include "../../../IApp.h"
#include <GLFW/glfw3.h>
#include <chrono>

GLFWwindow* s_window = nullptr;

bool Device::initialize()
{
	/* Initialize the library */
	if (!glfwInit()) return false;

	/* Create a windowed mode window and its OpenGL context */
	s_window = glfwCreateWindow(1280, 720, "BLINK", nullptr, nullptr);
	if (!s_window)
	{
		glfwTerminate();
		return false;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(s_window);

	return true;
}

void Device::terminate()
{
	glfwTerminate();
}

int Device::start(const UpdateCb& cb)
{
	auto begin = std::chrono::high_resolution_clock::now();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(s_window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		auto end = std::chrono::high_resolution_clock::now();
		auto duration = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		begin = end;
		cb(ms / 1000.0f);

		/* Swap front and back buffers */
		glfwSwapBuffers(s_window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	return 0;
}

int main(int argc, char** argv)
{
	return blink::IApp::mainEntry();
}
