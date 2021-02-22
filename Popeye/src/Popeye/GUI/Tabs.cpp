#include "pch.h"
#include "Tabs.h"
#include "../Component/ComponentManager.h"
#include "../System/RenderingSystem.h"
#include "../Scene/SceneManger.h"
#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

namespace Popeye{
	static GameObject* selectedGameObject;
	
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
		static Scene* scenes = SceneManager::GetInstance()->currentScene;
		CheckHover();
		if (ImGui::CollapsingHeader(scenes->GetName()))
		{
			for (int i = 0; i < scenes->gameObjects.size(); i++)
			{
				if (ImGui::Selectable(scenes->gameObjects[i]->GetName()))
				{
					scenes->GetAllComponents(scenes->gameObjects[i]->GetID());
					selectedGameObject = scenes->gameObjects[i];
				}
			}
		}
	}

	//Tab::Inspector
	void Inspector::ShowContents()
	{
		static ImGuiTextFilter filter;
		static bool addcomponentCall = false;

		CheckHover();
		if (selectedGameObject) // transform info first
		{
			if (ImGui::CollapsingHeader("Transform"))
			{
				ImGui::DragFloat3("position", (float*)&selectedGameObject->transform.position);
				ImGui::DragFloat3("rotation", (float*)&selectedGameObject->transform.rotation);
				ImGui::DragFloat3("scale", (float*)&selectedGameObject->transform.scale);
			}

			

			if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x, 0.0f)))
				addcomponentCall = true;
			
			if (addcomponentCall)
			{
				//ImGui::BeginMenuBar();
				//ImGui::BeginChild("search_component");
				filter.Draw("search");
				//ImGui::EndChild();
				//ImGui::EndMenuBar();
			}
		}
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
}