#pragma once
namespace Popeye {
	struct Tab;

	class GUIManager
	{
	private:
		ImGuiWindowFlags flags;
		ImGuiID dockspace_id;
		std::vector<Tab*> tabs;
	public:
		GUIManager();
		~GUIManager();
		void OnSet(GLFWwindow*);
		void SetTabs();
		void Set_default_layout();
		void OnRun();
		void Show();
		void OnClosed();
	};
}