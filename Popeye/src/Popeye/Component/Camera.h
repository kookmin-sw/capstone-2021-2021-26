#pragma once
namespace Popeye {
	enum class CamMod{ ORTHO, PERSPECTIVE };
	struct Cam
	{
		Cam();
		CamMod mod = CamMod::ORTHO;
		float fov = 45.0f;
		float nearView = 0.1f;
		float farView = 100.0f;
		float offsetX = 800.0f;
		float offsetY = 600.0f;
	};

	struct Camera
	{
		static std::unordered_map<int, Cam> cameras;

		static void ComponentAdded(int);
	};
}