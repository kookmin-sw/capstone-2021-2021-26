#include "Tabs.h"

#include "../Manager/ComponentManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/FileManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/RenderingComponents.h"

namespace Popeye{

	extern FileManager* g_fileManager;

	extern unsigned int g_SceneView;
	extern unsigned int g_GameView;

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

		ImGui::Image((ImTextureID)g_SceneView, wsize, ImVec2(0, 1), ImVec2(1, 0));

		CheckHover();

		ImGui::EndChild();

	}

	//Tab::GameView
	void GameView::ShowContents()
	{
		ImGui::BeginChild("Game Viewer");

		ImVec2 wsize = ImGui::GetWindowSize();

		ImGui::Image((ImTextureID)g_GameView, wsize, ImVec2(0, 1), ImVec2(1, 0));

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

				ImGui::ColorEdit3("color", (float*)&material.color);
				
				ImGui::DragFloat("ambient", &material.amb_diff_spec[0]);
				ImGui::DragFloat("diffuse", &material.amb_diff_spec[1]);
				ImGui::DragFloat("specular", &material.amb_diff_spec[2]);
				
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
			}
			else if (lightMod == 1) { 
				light.ChangeLightType(LightType::DIRECTION); 
			}
			else if (lightMod == 2) { 
				light.ChangeLightType(LightType::SPOT);
				
				ImGui::DragFloat("cutOff", &light.cutoff);
				ImGui::DragFloat("outerCutOff", &light.outercutoff);
			}
			
			ImGui::DragFloat("constant", &light.constant);
			ImGui::DragFloat("linear", &light.linear);
			ImGui::DragFloat("quadratic", &light.quadratic);

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
		static ImVec2 fileSize(70.0f, 80.0f);
		static ImVec2 offset(20.0f, 20.0f);

		CheckHover();
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiContext& g = *GImGui;

		std::vector<std::string> files = g_fileManager->ScanFilesInDir();
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;


		{
			ImGui::BeginChild("Dirs", ImVec2(150, 0), true);


			
			ImGui::EndChild();
		}
		ImGui::SameLine();

		{
			ImGui::BeginChild("FilesOfCurrentDir", ImVec2(0, 0), true);
			for (int i = 0; i < files.size(); i++)
			{
				const char* file_name = files[i].c_str();
				ImGui::PushID(i);

				ImGui::BeginGroup();

				ImGui::Selectable("##selectable", false, 0, fileSize);

				const ImVec2 p0 = ImGui::GetItemRectMin();
				const ImVec2 p1 = ImGui::GetItemRectMax();

				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				float centerPosX = (p0.x + p1.x) * 0.5f;

				ImVec2 imgRectMin = ImVec2(centerPosX - 30.0f, p0.y + 2.0f);
				ImVec2 imgRectMax = ImVec2(centerPosX + 30.0f, imgRectMin.y + 60.0f);

				draw_list->AddImage((ImTextureID)g_GameView, imgRectMin, imgRectMax);

				ImVec2 textsize = ImGui::CalcTextSize(file_name);
				float text_x_pos = textsize.x < fileSize.x ? centerPosX - (textsize.x * 0.5f) : centerPosX - (fileSize.x * 0.5f);
				ImVec2 text_pos = ImVec2(text_x_pos, imgRectMax.y);

				draw_list->AddText(g.Font, g.FontSize, text_pos, IM_COL32_WHITE, file_name, 0, textsize.x);

				ImGui::EndGroup();

				ImGui::PopID();

				float last_button_x2 = p1.x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x; // Expected position if next button was on same line
				if (i + 1 < files.size() && next_button_x2 < window_visible_x2)
					ImGui::SameLine();
			}
			ImGui::EndChild();
		}
	}
}