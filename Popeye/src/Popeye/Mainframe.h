#pragma once

namespace Popeye {
	class Mainframe
	{
		GLFWwindow* window;
	public:
		Mainframe();
		virtual ~Mainframe();

<<<<<<< HEAD
		bool init_Display(void);
		void run_Display(void);
		void close_Display(void);
=======
		bool Init(void);
		void Run(void);
		void Close(void);
>>>>>>> d3fea4472f4c492ecb6f3e9bd38da674be1267f9
	};
}

