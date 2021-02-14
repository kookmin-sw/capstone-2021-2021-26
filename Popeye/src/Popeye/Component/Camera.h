#pragma once
namespace Popeye {
	enum class Projection{ ORTHOGRAPHIC, PERSPECTIVE };
	struct Cam
	{
		Cam();
		Projection mod = Projection::ORTHOGRAPHIC;
		float fov = 45.0f;
		float nearView = 0.1f, farView = 100.0f;

		float offsetX = 800.0f, offsetY = 600.0f;
		
		float size = 20.0f;
	};

	struct Camera
	{
		static std::unordered_map<int, Cam> cameras;

		static void ComponentAdded(int);
	};
}