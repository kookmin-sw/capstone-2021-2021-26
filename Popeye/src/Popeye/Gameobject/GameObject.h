#pragma once
namespace Popeye {
	class GameObject
	{
	private:
		char* name;
		GameObject* parent;
		std::vector<GameObject*> childs;

		std::vector<class component*> components;
	public:
		GameObject();
		virtual ~GameObject();

		char* getName();
		char* setName(char*);

		GameObject* getParent();
		void setParent(GameObject*);

		GameObject* getchild();
		std::vector<GameObject*> getchildren();
		
		template<class component>
		component* addComponent();

		template<class component>
		component* getComponent();
		std::vector<class component*> getComponents();
	};
}

