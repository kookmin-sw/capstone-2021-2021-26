#include "UISystem.h"

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