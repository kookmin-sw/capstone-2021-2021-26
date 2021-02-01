#pragma once
namespace Popeye {
	class GameObject;
	
	class Scene //got gameobjects ID lists, if gameobject create, regist id. if gameobject deleted, 
	{
	private:
		int scene_id;
	public:
		Scene();
		~Scene();
	public:
		std::vector<int> gameobject_IDs;
		void GameObjectCreated(int);
		void GameObjectDeleted(int);
	};

	class SceneManager //load/ save scene :: make it near future...  
	{
	private:
		static SceneManager* instance;
		SceneManager();
		~SceneManager();
		std::vector<int> scene_IDs;
	public:
		Scene* currentScene;
		static SceneManager* GetInstance();
		static void DestroyInstance();
		int SceneCreated(Scene&);
		void LoadScene();
		void SaveScene();
	};
}
