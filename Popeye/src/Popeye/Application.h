#pragma once

#include "Core.h"
struct GLFWwindow {};
namespace Popeye {
	class POPEYE_API Application
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

