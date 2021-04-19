#include "GUIManager.h"

#include "../GUI/imgui_impl_glfw.h"
#include "../GUI/imgui_impl_opengl3.h"
#include "../GUI/IconsForkAwesome.h"
#include "../GUI/IconsForkAwesomeLargeIcon.h"

#include "../GUI/Tabs.h"
namespace Popeye
{
	ImFont *g_Icon;

	GUIManager::GUIManager() {}
	GUIManager::~GUIManager() { for (int i = 0; i < tabs.size(); i++) { delete(tabs[i]); } }

	void GUIManager::OnSet(GLFWwindow* window)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();

		io.Fonts->AddFontDefault();

		ImFontConfig config;
		config.MergeMode = true;
		config.OversampleH = 2;
		config.OversampleV = 1;
		config.GlyphExtraSpacing.x = 1.0f;

		static const ImWchar icon_ranges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
		io.Fonts->AddFontFromFileTTF("fonts/forkawesome-webfont.ttf", 13.0f, &config, icon_ranges);

		ImFontConfig configL;
		configL.MergeMode = false;
		configL.OversampleH = 2;
		configL.OversampleV = 1;
		configL.GlyphExtraSpacing.x = 1.0f;

		static const ImWchar icon_ranges_L[] = { ICON_FK_FOLDER_L, ICON_FK_FOLDER_L, ICON_FK_CODE_L,ICON_FK_CODE_L,  ICON_FK_TEXT_L,ICON_FK_TEXT_L, 
			ICON_FK_IMAGE_L,ICON_FK_IMAGE_L, ICON_FK_MODEL_L,ICON_FK_MODEL_L, 0 };

		g_Icon = io.Fonts->AddFontFromFileTTF("fonts/forkawesome-webfont.ttf", 50.0f, &configL, icon_ranges_L);

		io.Fonts->Build();

		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");

		flags = ImGuiWindowFlags_MenuBar;
		flags |= ImGuiWindowFlags_NoDocking;
		flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		SceneView *scene = new SceneView();
		scene->SetTab("Scene", 2);
		tabs.push_back(scene);

		GameView *game = new GameView();
		game->SetTab("Game", 3);
		tabs.push_back(game);

		Inspector *inspector = new Inspector();
		inspector->SetTab("Inspector");
		tabs.push_back(inspector);

		Hierarchy *hierarchy = new Hierarchy();
		hierarchy->SetTab("Hierarchy");
		tabs.push_back(hierarchy);

		Project *project = new Project();
		project->SetTab("Project");
		tabs.push_back(project);

		Resource *resource = new Resource();
		resource->SetTab("Resource");
		tabs.push_back(resource);
	}

	void GUIManager::OnRun()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		this->Show();
		
		ImGui::ShowDemoWindow();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void GUIManager::Show()
	{
		static int initialized = 0;
		
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

		float width = ImGui::GetWindowWidth();

		ImGui::Columns(3, "menu2", false);
		ImGui::Separator();
		ImGui::Button(ICON_FK_HAND_POINTER_O, ImVec2(25.0f, 25.0f)); ImGui::SameLine();
		ImGui::Button(ICON_FK_ARROWS, ImVec2(25.0f, 25.0f)); ImGui::SameLine();
		ImGui::Button(ICON_FK_SQUARE, ImVec2(25.0f, 25.0f));
		ImGui::NextColumn();
		ImGui::SameLine((ImGui::GetColumnWidth() / 2) - 26.0f);
		ImGui::Button(ICON_FK_PLAY, ImVec2(25.0f, 25.0f)); ImGui::SameLine(ImGui::GetColumnWidth() / 2);
		ImGui::Button(ICON_FK_STOP, ImVec2(25.0f, 25.0f)); ImGui::SameLine((ImGui::GetColumnWidth() / 2) + 26.0f);
		ImGui::Button(ICON_FK_FORWARD, ImVec2(25.0f, 25.0f));
		ImGui::NextColumn();
		/*TODO*/
		ImGui::NextColumn();
		ImGui::Columns(1);
		ImGui::Spacing();

		//ImGuiIO& io = ImGui::GetIO();
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");

		if (initialized == 0)
		{
			initialized = 1;
			ImGuiContext* ctx = ImGui::GetCurrentContext();
			ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
			ImGui::DockBuilderAddNode(dockspace_id, flags); // Add empty node

			ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
			ImGuiID dock_id_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.3f, NULL, &dock_main_id);
			ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.3f, NULL, &dock_main_id);
			ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 1.0f, NULL, &dock_main_id);

			ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
			ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
			ImGui::DockBuilderDockWindow("Scene", dock_main_id);
			ImGui::DockBuilderDockWindow("Game", dock_main_id);
			ImGui::DockBuilderDockWindow("Debug", dock_id_bottom);
			ImGui::DockBuilderDockWindow("Project", dock_id_bottom);
			ImGui::DockBuilderFinish(dockspace_id);
		}

		ImGui::DockSpace(dockspace_id);
		if (initialized == 1)
		{
			for (int i = 0; i < tabs.size(); i++)
			{
				tabs[i]->ShowTab();
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

	}

	void GUIManager::OnClosed()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

<<<<<<< HEAD:Popeye/src/Popeye/GUI/GUIManager.cpp

	//Tab 
	void Tab::SetTab(const char* _name, EventMod eventmod)
	{
		this->name = _name;
		this->eventmod = eventmod;
	}

	void Tab::CheckHover()
	{
		if (EventManager::GetInstance()->GetState() != eventmod)
		{
			if (ImGui::IsWindowHovered())
			{
				EventManager::GetInstance()->SetState(eventmod);
			}
		}
	}

	void Tab::ShowTab()
	{
		ImGui::Begin(this->name);
		
		ShowContents();

		ImGui::End();
	}

	void Tab::ShowContents() { }

	//Tab::sceneView
	void SceneView::ShowContents()
	{
		ImGui::BeginChild("Scene Viewer");

		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)RenderingSystem::worldTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));

		CheckHover();

		ImGui::EndChild();

	}

	//Tab::GameView
	void GameView::ShowContents()
	{
		ImGui::BeginChild("Game Viewer");

		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)RenderingSystem::viewTexture, wsize, ImVec2(0, 1), ImVec2(1, 0));

		CheckHover();

		ImGui::EndChild();
	}

	//Tab::Hierarchy
	void Hierarchy::ShowContents()
	{
		static Scene* scene = SceneManager::GetInstance()->currentScene;
		CheckHover();
		if (ImGui::CollapsingHeader(scene->GetName()))
		{
			for (int i = 0; i < scene->gameObjects.size(); i++)
			{
				if (ImGui::Selectable(scene->gameObjects[i]->GetName()))
				{
					std::cout << scene->gameObjects[i]->GetName() << std::endl;
				}
			}
		}
	}
	
	//Tab::Inspector
	void Inspector::ShowContents()
	{
		CheckHover();
	}

	//Tab::Debug
	void Debug::ShowContents()
	{
		CheckHover();
	}

	//Tab::Project
	void Project::ShowContents()
	{
		CheckHover();
	}

=======
>>>>>>> d3fea4472f4c492ecb6f3e9bd38da674be1267f9:Popeye/src/Popeye/Manager/GUIManager.cpp
}