#pragma once

namespace Popeye {
	class Mainframe
	{
		GLFWwindow* window;
		static bool exit;
	public:
		Mainframe();
		virtual ~Mainframe();

		bool init_Display(void);
		void run_Display(void);
		void close_Display(void);
	};
}

