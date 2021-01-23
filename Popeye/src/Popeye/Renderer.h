#pragma once

typedef struct GLFWwindow{};
namespace Popeye {
	class Renderer
	{
		GLFWwindow* Window;
		static bool exit;
	public:
		Renderer();
		virtual ~Renderer();

		GLFWwindow* get_window(void);

		bool init_renderer(void);
		void run_renderer(void);
		void close_renderer(void);
	};
}

