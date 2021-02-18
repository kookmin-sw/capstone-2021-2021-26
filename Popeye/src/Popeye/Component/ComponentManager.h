#pragma once

namespace Popeye {

	class Datatable
	{
	public:
		Datatable();
		virtual ~Datatable() = 0;
	};

	template<class component>
	class ComponentDatatable : public Datatable
	{
	private:
		std::vector<component> componentDatatable;
	public:
		component GetData(int key){ return componentDatatable[key]; }
	};

	class ComponentManager
	{
	private:
		std::vector<Datatable> components;
	public:
		template<class component>
		void RegistComponent()
		{
			components.push_back(ComponentDatatable<component>);
		}
	};
}