#pragma once
namespace Popeye {
	class GameObject
	{
	private:
		int ID;
		char* name;
		GameObject* parent;
		std::vector<GameObject*> childs;
	public:
		GameObject(int);
		virtual ~GameObject();

		char* GetName();
		char* SetName(char*);

		GameObject* GetParent();
		void SetParent(GameObject*);

		GameObject* Getchild();
		std::vector<GameObject*> Getchildren();
		
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

