#pragma once

namespace Popeye {
	enum EventMod;
	class EventSystem
	{
	private:
		EventMod eventstate;
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();;
	};

}