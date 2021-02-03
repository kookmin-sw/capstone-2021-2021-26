#pragma once
namespace Popeye {
	class RenderingSystem
	{
	private:
		std::vector<unsigned int> VAOs;
	
	public:
		unsigned int sceneVAO;
		unsigned int FBO;
		unsigned int viewTexture;
	
	private :
		void InitBufer();
	
	public:
		void SystemRunning();
		void Init();
		void OnUpdate();
		void OnExit();
	};
}

