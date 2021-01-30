#include "pch.h"
#include "RenderingSystem.h"

#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"

namespace Popeye {
	
	// render system should run while not on play...
	void RenderingSystem::OnEdit()
	{
		for (int i = 0; i < Transform::position.size(); i++)
		{
		}
	}

	// On play -> init -> update -> on exit....
	void RenderingSystem::Init()
	{
	}

	void RenderingSystem::OnUpdate()
	{
	}

	void RenderingSystem::OnExit()
	{

	}
}