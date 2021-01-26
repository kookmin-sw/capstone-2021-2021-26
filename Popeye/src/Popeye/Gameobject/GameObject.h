#pragma once
namespace Popeye {
	//class Component {};
	class GameObject
	{
	private:
		char* name;
		GameObject* parent;
		std::vector<GameObject*> childs;
		//std::unordered_map<size_t, Component*> components;
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
		void addComponent()
		{
			/*size_t id = component::getID();
			if (this->components.find(id) == this->components.end())
			{
				component* comp = new component();
				this->components[id] = comp;
			}
			else
			{
				std::cout << "test, already exist component!" << std::endl;
			}*/
		}

		template<class component>
		component* getComponent()
		{
			size_t id = component::getID();
			if (this->components.find(id) == this->components.end())
				return nullptr;
			return reinterpret_cast<component*>(this->components[id]);
		}

		template<class component>
		std::vector<component*> getComponents();
	};
}

