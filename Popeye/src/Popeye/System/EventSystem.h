#pragma once
#include "../Events/EventManager.h"
namespace Popeye {

	class EventSystem
	{
	private:
		EventMod eventstate;
	private:
		void GUIEvent();
		void SceneEvent();
		void InputEvent();
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();;
	};

}