#pragma once
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Engine {
	void GUI(bool* p_open);
	void tab(char* _name);
	class Tab
	{
	private:
	char* name;
	public:
		Tab();
		virtual ~Tab();
		void setTab(char* _name);
		void showTab();
	};

	class Property : Tab
	{

	};

	class GUIManager
	{
	private:
		ImGuiID dockspace_id;
		//ImGuiIO&
		std::vector<Tab> tabs;
	public:
		GUIManager();
		virtual ~GUIManager();
		void onSet(GLFWwindow* window);
		void setTabs();
		void set_default_layout();
		void onRun();
		void show();
		void onClosed();
	};
}