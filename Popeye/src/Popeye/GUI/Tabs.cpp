#include "pch.h"
#include "Tabs.h"
#include "../Component/ComponentManager.h"
#include "../System/RenderingSystem.h"
#include "../Scene/SceneManger.h"
#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

namespace Popeye{
	static GameObject* selectedGameObject;
	static Scene* scene;
	//Tab 
	void Tab::SetTab(const char* _name, EventMod _eventmod)
	{
		name = _name;
		eventmod = _eventmod;
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
		ImGui::Begin(name);

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
		scene = SceneManager::GetInstance()->currentScene;
		CheckHover();
		if (ImGui::CollapsingHeader(scene->GetName()))
		{
			for (int i = 0; i < scene->gameObjects.size(); i++)
			{
				if (ImGui::Selectable(scene->gameObjects[i]->GetName()))
				{
					selectedGameObject = scene->gameObjects[i];
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

			/*Show all Component*/
			int id = selectedGameObject->GetID();
			std::vector<Accessor> accessor = scene->GetAllAddressOfID(id); // TODO::Make copy once.

			for (int i = 0; i < accessor.size(); i++)
			{
				if (accessor[i].componentType != nullptr)
				{
					if (accessor[i].componentType == typeid(Camera).name())
					{
						ShowComponent(selectedGameObject->GetComponent<Camera>());
					}
					if (accessor[i].componentType == typeid(MeshRenderer).name())
					{
						ShowComponent(selectedGameObject->GetComponent<MeshRenderer>());
					}
					if (accessor[i].componentType == typeid(Light).name())
					{
						ShowComponent(selectedGameObject->GetComponent<Light>());
					}
				}
			}

			/*Add Component*/
			if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x, 0.0f)))
				addcomponentCall = true;
			
			if (addcomponentCall)
			{
				filter.Draw("search");
			}
		}
	}


	void Inspector::ShowComponent(Camera& camera)
	{
		const char* camMod[] = { "Perspective", "Otrhomatric" };

		int cameraMod = -1;
		if (camera.mod == Projection::PERSPECTIVE) { cameraMod = 0; }
		if (camera.mod == Projection::ORTHOGRAPHIC) { cameraMod = 1; }

		if (ImGui::CollapsingHeader("Camera"))
		{
			ImGui::Combo("Projection", &cameraMod, camMod, IM_ARRAYSIZE(camMod), IM_ARRAYSIZE(camMod)); // TODO::Make look pretty.
			if (cameraMod == 0) {
				camera.mod = Projection::PERSPECTIVE;
				ImGui::DragFloat("fov", &camera.fov);
				ImGui::DragFloat("offset x", &camera.offsetX);
				ImGui::DragFloat("offset y", &camera.offsetY);
			}
			if (cameraMod == 1) {
				camera.mod = Projection::ORTHOGRAPHIC;
				ImGui::DragFloat("width", &camera.width);
				ImGui::DragFloat("height", &camera.height);
			}

			ImGui::DragFloat("far view", &camera.farView);
			ImGui::DragFloat("near view", &camera.nearView);
		}
	}

	void Inspector::ShowComponent(MeshRenderer& meshRenderer)
	{
		if (ImGui::CollapsingHeader("MeshRenderer"))
		{
			if (ImGui::TreeNode("Mesh"))
			{
				ImGui::TreePop();
			}
			if (ImGui::TreeNode("Material"))
			{
				Material& material = MeshRenderer::materials[meshRenderer.materialIndex];

				ImGui::ColorEdit3("material color", (float*)&material.color);
				
				ImGui::DragFloat("ambiant",	&material.ambient);
				ImGui::DragFloat("diffuse",	&material.diffuse);
				ImGui::DragFloat("specular",&material.specular);
				ImGui::DragFloat("shininess",	&material.shininess);
				
				ImGui::TreePop();
			}
		}
	}

	void Inspector::ShowComponent(Light& light)
	{
		const char* lighttype[] = { "Point", "Direction", "Spot" };

		int lightMod = -1;
		LightType type = light.ShowLightType();
		
		if (type == LightType::POINT) { lightMod = 0; }
		else if (type == LightType::DIRECTION) { lightMod = 1; }
		else if (type == LightType::SPOT) { lightMod = 2; }

		if (ImGui::CollapsingHeader("Light"))
		{
			ImGui::Combo("Light type", &lightMod, lighttype, IM_ARRAYSIZE(lighttype), IM_ARRAYSIZE(lighttype)); // TODO::Make look pretty.
			if (lightMod == 0)		{ 
				light.ChangeLightType(LightType::POINT);

				ImGui::DragFloat("constant", &light.constant);
				ImGui::DragFloat("linear", &light.linear);
				ImGui::DragFloat("quadratic", &light.quadratic);
			}
			else if (lightMod == 1) { 
				light.ChangeLightType(LightType::DIRECTION); 
			}
			else if (lightMod == 2) { 
				light.ChangeLightType(LightType::SPOT);

				ImGui::DragFloat("constant", &light.constant);
				ImGui::DragFloat("linear", &light.linear);
				ImGui::DragFloat("quadratic", &light.quadratic);

				ImGui::DragFloat("cutOff", &light.cutoff);
				ImGui::DragFloat("outerCutOff", &light.outercutoff);
			}

			ImGui::ColorEdit3("light color", (float*)&light.color);
			
			ImGui::DragFloat("ambient", &light.ambient);
			ImGui::DragFloat("diffuse", &light.diffuse);
			ImGui::DragFloat("specular",&light.specular);
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