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
		bool window_should_close();
		bool poll_events();
		void run_renderer();
		void swap_buffer();
		void close_renderer();

	};
}

