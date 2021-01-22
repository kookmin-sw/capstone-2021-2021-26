#include "pch.h"
#include "Renderer.h"

namespace Popeye {
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

	bool Renderer::window_should_close()
	{
		if (glfwWindowShouldClose(Window))
		{
			return true;
		}
		return false;
	}

	GLFWwindow* Renderer::get_window()
	{
		return this->Window;
	}

	bool Renderer::poll_events()
	{
		glfwPollEvents();

		return glfwWindowShouldClose(Window);
	}

	void Renderer::run_renderer()
	{
		int display_w, display_h;
		glfwGetFramebufferSize(Window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.5f, 0.5f, 0.5f, 0.25f);
		glClear(GL_COLOR_BUFFER_BIT);
	}
	void Renderer::swap_buffer()
	{
		glfwSwapBuffers(Window);
	}


	void Renderer::close_renderer()
	{
		glfwTerminate();
	}
}