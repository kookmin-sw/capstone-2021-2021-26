#pragma once
namespace Popeye {

	class EventSystem
	{
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();
	};
}