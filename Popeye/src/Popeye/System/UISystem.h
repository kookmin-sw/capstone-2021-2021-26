#pragma once

namespace Popeye {
	class UISystem
	{
		bool* onPlay;
	public:
		void SystemInit(bool* _onPlay);
		void SystemRunning();
	};
}

