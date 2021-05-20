#pragma once
namespace Popeye {

	class RenderingSystem
	{
	private:
		unsigned int gameViewFBO;

		void RenderGameView();
	public:
		void SystemInit();
		void SystemRunning();
	};
}

