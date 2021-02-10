#pragma once

namespace Popeye {
	enum class EventMod 
	{
		ONGUI,
		SCENE,
		GAME
	};

	class EventManager
	{
	private:
		static EventManager* instance;
		EventManager();
		~EventManager();
	private:
		EventMod eventstate = EventMod::ONGUI;
	public:
		static EventManager* GetInstance();
		static void DestroyInstance();
	
		void SetState(EventMod newState);
		EventMod GetState() const; 
	};
}

