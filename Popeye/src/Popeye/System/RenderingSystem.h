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
	public:
		//screenCam pos, dir
		glm::vec3 *sceneViewPos;
		glm::vec3 *sceneViewDir;
		//ray
		bool *sendEditRay;
		glm::vec3 *screenToMousePos;
		glm::vec3 *screenToMouseDir;
	private :
		void RenderSceneView();
		void RenderGameView();
	public:
		void SystemInit();
		void SystemRunning();
	};
}

