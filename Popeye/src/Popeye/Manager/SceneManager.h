#pragma once

namespace Popeye {
	class Scene;

	class SceneManager //load/ save scene :: make it near future...  
	{
	private:
		static SceneManager* instance;
		SceneManager();
		~SceneManager();
	public:
		Scene* currentScene;
		std::string currentScenePath;
	public:
		static SceneManager* GetInstance();
		static void DestroyInstance();
		void CreateScene();
		void UnloadScene();
	};
}
