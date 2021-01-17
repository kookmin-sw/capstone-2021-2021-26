#include "pch.h"
#include "GUI.h"

namespace Engine
{
    void GUI(bool* p_open)
    {
		static int initialized = 0;
		static int new_window = 0;
		ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", 0, flags);
		ImGui::PopStyleVar();

		if (ImGui::BeginMenuBar())
		{
			
			ImGui::EndMenuBar();
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

		if (initialized == 0)
		{
			initialized = 1;
			ImGuiContext* ctx = ImGui::GetCurrentContext();
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, flags); // Add empty node

			ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.10f, NULL, &dock_main_id);
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);
			ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.50f, NULL, &dock_main_id);

			//ImGui::DockBuilderDockWindow(dock_main_id, "S");

			ImGui::DockBuilderDockWindow("Log", dock_id_bottom);
			ImGui::DockBuilderDockWindow("Properties", dock_id_left);
			ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
			ImGui::DockBuilderDockWindow("Scene", dock_main_id);
			ImGui::DockBuilderFinish(dockspace_id);
		}

		ImGui::DockSpace(dockspace_id);
		if (initialized == 1)
		{

			ImGui::Begin("Log");
			ImGui::End();

			ImGui::Begin("Properties");
			ImGui::End();

			ImGui::Begin("Inspector");
			ImGui::End();

			ImGui::Begin("Scene");
			ImGui::End();
		}

		if (new_window == 1)
		{
			// Should dock window to empty space, instead window is not docked anywhere.
			//ImGui::SetNextWindowDockId(dockspace_id, ImGuiCond_Once);
			ImGui::Begin("New Window");
			ImGui::End();
		}

		ImGui::End();
		ImGui::PopStyleVar();
    }
}