#include "UIComponents.h"

namespace Popeye
{
	UIFrame::UIFrame()
	{
		SetValue();
	}

	void UIFrame::SetValue()
	{
		textureID = -1;
		leftTop = glm::vec2(0.0f);
		rightBot = glm::vec2(0.0f);
	}

	Text::Text()
	{
		SetValue();
	}

	void Text::SetValue()
	{
		contents = " ";
		leftTop = glm::vec2(0.0f);
		rightBot = glm::vec2(0.0f);
	}
}