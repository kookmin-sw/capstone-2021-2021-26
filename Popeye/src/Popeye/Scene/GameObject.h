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
		char* name;
		GameObject* parent;
		std::vector<GameObject*> childs;
	public:
		Transform transform;
	public:
		GameObject(int);
		~GameObject();

		char* GetName();
		void SetName(char*);

		GameObject* GetParent();
		void SetParent(GameObject*);

		GameObject* Getchild();
		std::vector<GameObject*> Getchildren();
		void AddChild(GameObject*);

		int GetID();
		
		template<class component>
		void AddComponent()
		{
			component::ComponentAdded(id);
		}

		template<class component>
		component& GetComponent()
		{
			return component(); //todo :: think about how to access
		}

	};
}

