#pragma once

namespace Popeye{

	struct Point{
		float x;
		float y;
	};

	struct UIFrame{
		glm::vec2 leftTop;
		glm::vec2 rightBot;
	};

	struct Text{
		std::string contents;
		glm::vec2 leftTop;
		glm::vec2 rightBot;
	};

	struct Button{
		glm::vec2 leftTop;
		glm::vec2 rightBot;
	};

}
