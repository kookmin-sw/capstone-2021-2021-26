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

	GLFWwindow* Renderer::get_window()
	{
		return this->Window;
	}

	void Renderer::run_renderer()
	{
		static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,1.0f,0.0f,
		};

		static GUIManager* guimanager = new GUIManager();

		guimanager->onSet(this->get_window());

		while (!glfwWindowShouldClose(Window))
		{
			//guimanager->onRun();
			int display_w, display_h;
			glfwGetFramebufferSize(Window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(0.5f, 0.5f, 0.5f, 0.25f);

			glClear(GL_COLOR_BUFFER_BIT);
			//guimanager->onRunDraw();
			glfwSwapBuffers(Window);
			glfwPollEvents();
		}

		delete(guimanager);
	}

	void Renderer::close_renderer()
	{
		glfwTerminate();
	}
}