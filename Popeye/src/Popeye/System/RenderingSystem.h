#pragma once
namespace Popeye {
	struct Texture;
	struct Shader;
	struct Cam;

	enum class RenderState
	{
		RENDERSCENEVIEW,
		RENDERGAMEVIEW
	};

	class RenderingSystem
	{
	private:
		std::vector<unsigned int> VAOs;
	public:
		unsigned int worldViewFBO, FBO;
		static unsigned int viewTexture;
		static unsigned int worldTexture;
	
	private :
		void Rendering(RenderState&);
	
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();
	};
}

