#pragma once
namespace Popeye {
	template<class _component>
	struct _component_id {
		typedef _component component;
	};

#define DEFINE_COMPONENT(component_name)\
	public: \
		typedef _component_id<component_name> component_identifier; \
		static size_t getID(void){return reinterpret_cast<size_t>(&identifier);} \
	private: \
		static component_identifier identifier; \

#define DECLARE_COMPONENT(component_name) \
	component_name::component_identifier component_name::identifier;

	class Component
	{
		DEFINE_COMPONENT(Component);
	public:
		Component();
		~Component();
		void test();
	};

	class MeshRenderer : Component
	{
		DEFINE_COMPONENT(MeshRenderer);
	public:
		MeshRenderer();
		~MeshRenderer();
	};

	DECLARE_COMPONENT(Component);
	DECLARE_COMPONENT(MeshRenderer);
}

