#pragma once
namespace Popeye {
	class EventSystem
	{
	public:
		void SetInputCallbacks(GLFWwindow*);
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();
	};
}

