#include "PhysicsSystem.h"
namespace Popeye
{
	void PhysicsSystem::SystemInit(bool* _onPlay)
	{
		onPlay = _onPlay;
	}
	
	void PhysicsSystem::SystemRunning() 
	{

		if (onPlay[0])
		{
			POPEYE_CORE_INFO("Popeye Physics");
		}

	}
}