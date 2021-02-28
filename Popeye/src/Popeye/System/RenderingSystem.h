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
		unsigned int worldViewFBO, gameViewFBO;
	public:
		static unsigned int viewTexture;
		static unsigned int worldTexture;
	
	private :
		void Rendering();
	
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();
	};
}

