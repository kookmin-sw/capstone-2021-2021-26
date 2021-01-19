#include "pch.h"
#include "Application.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	Application::Application()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Engine::Log::Init();

		window = glfwCreateWindow(1200, 600, "Popeye Engine", NULL, NULL);
		if (!window)
		{
			ENGINE_CORE_ERROR("Failed to create GLFW window.");
			return;
		}
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			ENGINE_CORE_ERROR("Failed to initialize GLAD.");
			return;
		}
        
		Engine::GUIManager* guiManager = new GUIManager();
		guiManager->onSet(window);
		
        static bool show_window = true;
        static bool show = true;
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Start the Dear ImGui frame
			guiManager->onRun();

            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.5f, 0.5f, 0.5f, 0.25f);
            glClear(GL_COLOR_BUFFER_BIT);
            
            glfwSwapBuffers(window);
        }

        // Cleanup
		guiManager->onClosed();
		delete guiManager;

		glfwTerminate();
		return;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
