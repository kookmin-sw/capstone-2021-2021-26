#pragma once
namespace Popeye {

	struct Tab
	{
		const char* name;
		void setTabName(const char*);
		void showTab();
		virtual void showContents();
	};

	struct Property : public Tab
	{
		virtual void showContents();
	};

	struct SceneView : public Tab
	{
		virtual void showContents();
	};

	class GUIManager
	{
	private:
		ImGuiWindowFlags flags;
		ImGuiID dockspace_id;
		//ImGuiIO&
		std::vector<Tab*> tabs;
	public:
		GUIManager();
		~GUIManager();
		void onSet(GLFWwindow*);
		void setTabs();
		void set_default_layout();
		void onRun();
		void onRunDraw();
		void show();
		void onClosed();
	};
}