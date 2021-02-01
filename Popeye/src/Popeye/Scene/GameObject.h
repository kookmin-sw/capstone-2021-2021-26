#pragma once
namespace Popeye {
	class Scene;

	class GameObject
	{
	private:
		int ID;
		char* name;
		GameObject* parent;
		std::vector<GameObject*> childs;
	public:
		GameObject();
		virtual ~GameObject();

		char* GetName();
		char* SetName(char*);

		GameObject* GetParent();
		void SetParent(GameObject*);

		GameObject* Getchild();
		std::vector<GameObject*> Getchildren();

		int GetID();
		
		template<class component>
		void AddComponent()
		{
			component::ComponentAdded(this->ID);
		}

		template<class component>
		component GetComponent()
		{
			return component(); //todo :: think about how to access
		}

	};
}

