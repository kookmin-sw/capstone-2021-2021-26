#pragma once

#include "Core.h"
struct GLFWwindow {};
namespace Engine {
	class ENGINE_API Application
	{
		GLFWwindow* window;
	public:
		Application();
		virtual ~Application();
		void Run();
		//struct* window;
	};

	Application* CreateApplication();
}

