#include "pch.h"
#include "GUIManager.h"

namespace Engine
{
	

	void tab(char* name)
	{
		ImGui::Begin(name);
		ImGui::End();
	}

	GUIManager::GUIManager() {}
	GUIManager::~GUIManager() {}
	void GUIManager::onSet(GLFWwindow *window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io = ImGui::GetIO(); (void)io;

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsClassic();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		flags = ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_NoDocking;
		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	void GUIManager::onRun()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		this->show();

		ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUIManager::onRunDraw()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUIManager::init_view()
	{
	}

	void GUIManager::show()
	{
		static int initialized = 0;
		static int new_window = 0;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", 0, flags);
		ImGui::PopStyleVar();

		static bool new_project = false;
		static bool save_project = false;
		static bool load_project = false;

		static bool create_entity = false;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("file"))
			{
				ImGui::MenuItem("new project", NULL, &new_project);
				ImGui::MenuItem("save project", NULL, &save_project);
				ImGui::MenuItem("load project", NULL, &load_project);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("gameobject"))
			{
				ImGui::MenuItem("create entity", NULL, &create_entity);
				ImGui::EndMenu();
			}
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
			ImGui::DockBuilderDockWindow("Project viewer", dock_id_bottom);
			ImGui::DockBuilderDockWindow("Properties", dock_id_left);
			ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
			ImGui::DockBuilderDockWindow("Scene", dock_main_id);
			ImGui::DockBuilderFinish(dockspace_id);
		}

		ImGui::DockSpace(dockspace_id);
		if (initialized == 1)
		{
			/*tab("Log");

			tab("Properties");

			tab("Inspector");

			tab("Scene");

			tab("Project viewer");*/
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

	void GUIManager::onClosed()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	Tab::Tab() {}
	Tab::~Tab() {}
	void Tab::setTab(char* _name)
	{
		this->name = _name;
	}
	void Tab::showTab()
	{
		ImGui::Begin(this->name);

		ImGui::End();
	}
}