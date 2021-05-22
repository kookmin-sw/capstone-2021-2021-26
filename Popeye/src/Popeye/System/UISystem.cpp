#include "UISystem.h"

#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/ComponentManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/UIComponents.h"

namespace Popeye
{
	void UISystem::SystemInit(bool* _onPlay)
	{
		onPlay = _onPlay;
	}

	void UISystem::SystemRunning()
	{

		if (onPlay[0])
		{
			POPEYE_CORE_INFO("Popeye UI");
		}
	}
}