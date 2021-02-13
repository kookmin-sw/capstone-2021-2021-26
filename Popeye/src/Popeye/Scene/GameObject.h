#pragma once
namespace Popeye {
	class Scene;

	class GameObject
	{
	private:
		int ID;
		char* name;
		int parentID;
		std::vector<int> childs;
	public:
		GameObject();
		~GameObject();

		char* GetName();
		void SetName(char*);

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
		component& GetComponent()
		{
			return component(); //todo :: think about how to access
		}

	};
}

