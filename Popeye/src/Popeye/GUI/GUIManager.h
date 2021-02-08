#pragma once
namespace Popeye {
	struct Tab
	{
		const char* name;
		void SetTabName(const char*);
		void ShowTab();
		virtual void ShowContents();
	};

	struct Hierarchy : public Tab
	{
		virtual void ShowContents();
	};

	struct Inspector : public Tab
	{
		virtual void ShowContents();
	};

	struct SceneView : public Tab
	{
		virtual void ShowContents();
	};

	struct GameView : public Tab 
	{
		virtual void ShowContents();
	};

	struct Debug : public Tab
	{
		virtual void ShowContents();
	};

	struct Project : public Tab
	{
		virtual void ShowContents();
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
		void OnSet(GLFWwindow*);
		void SetTabs();
		void Set_default_layout();
		void OnRun();
		void Show();
		void OnClosed();
	};
}