#pragma once

namespace Popeye {
	class ScriptingSystem
	{
		bool* onPlay;
	public:
		void SystemInit(bool* _onPlay);
		void SystemRunning();
	};
}

