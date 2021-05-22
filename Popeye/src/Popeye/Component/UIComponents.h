#pragma once

namespace Popeye{

	struct UIFrame{
		unsigned int textureID;
		glm::vec2 leftTop;
		glm::vec2 rightBot;

		UIFrame();
		void SetValue();
	};

	struct Text{
		std::string contents;
		glm::vec2 leftTop;
		glm::vec2 rightBot;

		Text();
		void SetValue();
	};

	struct Button{
		glm::vec2 leftTop;
		glm::vec2 rightBot;

		Button();
		void SetValue();
	};

}
