#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Engine {
	class Renderer
	{
		GLFWwindow* Window;
		static bool exit;
	public:
		Renderer();
		virtual ~Renderer();

		GLFWwindow* get_window();

		bool init_renderer();
		bool run_renderer();
		void close_renderer();

	};
}

