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
		Engine::Log::Init();
		bool window_should_close = false;
		bool initialize = false;
		Engine::Renderer* renderer = new Renderer();

		initialize = renderer->init_renderer();
		if (!initialize)
		{
			delete(renderer);
			ENGINE_CORE_ERROR("Renderer Initialize fail.");
			return;
		}

		Engine::GUIManager* guiManager = new GUIManager();
		guiManager->onSet(renderer->get_window());
		
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        // Main loop
        while (!window_should_close)
        {

            // Start the Dear ImGui frame
			guiManager->onRun();

			window_should_close = renderer->run_renderer();
            
        }

        // Cleanup
		guiManager->onClosed();
		delete guiManager;

		renderer->close_renderer();
		delete renderer;
		return;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

}
