#pragma once

namespace Popeye {
	class Display
	{
		GLFWwindow* Window;
		static bool exit;
	public:
		Display();
		virtual ~Display();

		GLFWwindow* get_window(void);

		bool init_Display(void);
		void run_Display(void);
		void close_Display(void);
	};
}

