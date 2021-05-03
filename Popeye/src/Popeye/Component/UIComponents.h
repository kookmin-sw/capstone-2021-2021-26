#pragma once

namespace Popeye{

	glm::vec2

	struct UIFrame{
		Point leftTop;
		Point rightBot;
	};

	struct Text{
		std::string contents
		Point leftTop;
		Point rightBot;
	};

	struct Button{
		Point leftTop;
		Point rightBot;
	};

}
