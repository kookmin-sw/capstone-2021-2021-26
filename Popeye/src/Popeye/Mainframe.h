#pragma once

namespace Popeye {
	class Mainframe
	{
		GLFWwindow* window;
	public:
		Mainframe();
		virtual ~Mainframe();

		bool Init(void);
		void Run(void);
		void Close(void);
	};
}

