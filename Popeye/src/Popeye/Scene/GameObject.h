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

		char* getName();
		char* setName(char*);

		GameObject* getParent();
		void setParent(GameObject*);

		GameObject* getchild();
		std::vector<GameObject*> getchildren();
		
		template<class component>
		void addComponent()
		{
			component::componentAdded(this->ID);
		}

		template<class component>
		component getComponent()
		{
			return component(); //todo :: think about how to access
		}
	};
}

