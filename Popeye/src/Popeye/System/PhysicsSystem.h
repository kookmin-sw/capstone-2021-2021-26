#pragma once

namespace Popeye {
	class PhysicsSystem
	{
		bool *onPlay;
	public:
		void SystemInit(bool *_onPlay);
		void SystemRunning();
	};
}

