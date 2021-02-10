#pragma once

namespace Popeye {
	enum EventMod 
	{
		NONE,
		EDIT,
		INPUT
	};

	class EventManager
	{
	private:
		static EventManager* instance;
		EventManager();
		~EventManager();
	private:
		EventMod eventstate = EventMod::NONE;
	public:
		static EventManager* GetInstance();
		static void DestroyInstance();
	
		void SetState(EventMod newState);
		EventMod GetState() const; 
	};
}

