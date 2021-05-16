#pragma once
namespace Popeye {

	struct Transform
	{
		glm::vec3 position	= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation	= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale		= glm::vec3(1.0f, 1.0f, 1.0f);
	};

	class GameObject
	{
	private:
		int id;
		std::string name;
		/*GameObject* parent;
		std::vector<GameObject*> childs;*/
	public:
		Transform transform;
	public:
		GameObject(int _id);
		~GameObject();

		void SetValue(int _id);

		std::string GetName();
		void SetName(std::string);

		/*GameObject* GetParent();
		void SetParent(GameObject*);

		GameObject* Getchild();
		std::vector<GameObject*> Getchildren();
		void AddChild(GameObject*);*/

		int GetID();
		
		template<class component>
		void AddComponent()
		{
			SceneManager::GetInstance()->currentScene->AddData<component>(id);
		}

		template<class component>
		void DeleteComponent()
		{
			SceneManager::GetInstance()->currentScene->RemoveData<component>(id);
		}

		void AddComponentByName(const char* component);

		template<class component>
		component& GetComponent()
		{
			return SceneManager::GetInstance()->currentScene->GetData<component>(id);; //todo :: think about how to access
		}

	};
}

