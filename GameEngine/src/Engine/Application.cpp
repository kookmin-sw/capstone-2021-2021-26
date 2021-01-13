#include "pch.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
namespace Engine {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		GLFWwindow* window;
		if (!glfwInit())
			return;
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


		window = glfwCreateWindow(1200, 600, "Popeyr Engine", NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			return;
		}

		glfwMakeContextCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return ;
		}
		glfwSwapInterval(1);

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.2f, 0.1f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glfwSwapBuffers(window);

			glfwPollEvents();
			
		}

		glfwTerminate();
		return;
	}
}
