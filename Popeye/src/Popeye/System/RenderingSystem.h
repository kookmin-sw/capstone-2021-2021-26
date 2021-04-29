#pragma once
namespace Popeye {

	enum class RenderState
	{
		RENDERSCENEVIEW,
		RENDERGAMEVIEW
	};

	class RenderingSystem
	{
	private:
		RenderState renderstate;
		unsigned int sceneViewFBO, gameViewFBO;
	private :
		void RenderSceneView();
		void RenderGameView();
	public:
		void SystemInit();
		void SystemRunning();
	};
}

