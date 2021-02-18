#pragma once

namespace Popeye {

	struct testComponent0 { int x = 0; int y = 1; int z = 3; };
	struct testComponent1 { bool blahblah = true; };
	struct testComponent2 {};
	struct testComponent3 {};
	struct testComponent4 {};

	class BaseDatatable
	{
	public:
		virtual ~BaseDatatable() = default;
	};

	template<class component>
	class ComponentDatatable : public BaseDatatable
	{
	private:
		std::vector<component> componentDatatable;
	public:
		component GetData(int key){ return componentDatatable[key]; }
	};

	class ComponentManager
	{
	private:
		std::vector<BaseDatatable*> datas;
	public:
		template<typename component>
		void RegistComponent()
		{
			datas.push_back(new ComponentDatatable<component>());
		}
	};
}