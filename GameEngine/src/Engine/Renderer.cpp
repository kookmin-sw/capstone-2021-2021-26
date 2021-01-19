#include "pch.h"
#include "Renderer.h"

namespace Engine {
	Renderer::Renderer(){}
	Renderer::~Renderer(){}
	
	bool Renderer::init_renderer()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->Window = glfwCreateWindow(1200, 600, "Popeye Engine", NULL, NULL);
		if (!this->Window)
		{
			return false;
		}
		glfwMakeContextCurrent(this->Window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}
		return true;
	}

	GLFWwindow* Renderer::get_window()
	{
		return this->Window;
	}


	bool Renderer::run_renderer()
	{
		glfwPollEvents();
		if (glfwWindowShouldClose(this->Window))
		{
			return true;
		}

		glViewport(0, 0, 1200, 600);
		glClearColor(0.5f, 0.5f, 0.5f, 0.25f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(Window);
		return false;
	}

	void Renderer::close_renderer()
	{
		glfwTerminate();
	}
}