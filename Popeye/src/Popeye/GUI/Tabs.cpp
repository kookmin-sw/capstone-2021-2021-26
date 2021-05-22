#include "Tabs.h"

#include "IconsForkAwesome.h"
#include "IconsForkAwesomeLargeIcon.h"

#include "../Manager/GUIManager.h"
#include "../Manager/ComponentManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../FileIO.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"

#include "../Component/RenderingComponents.h"
#include "../Component/physicsComponents.h"
#include "../Component/UIComponents.h"

namespace Popeye{

	extern bool				isPlay;
	extern ResourceManager	*g_ResourceManager;
	extern ImFont			*g_Icon;
	extern FileIO			*g_fileIO;
		
	extern int				g_eventMod;
	extern unsigned int		g_SceneView;
	extern unsigned int		g_GameView;
	
	extern glm::vec2		g_scenePosition;
	extern glm::vec2		g_sceneSize;

	Scene		*scene;
	GameObject	*selectedGameObject = nullptr;


	//Tab 
	void Tab::SetTab(const char* _name, int _eventmod)
	{
		name = _name;
		eventmod = _eventmod;
	}

	void Tab::CheckHover()
	{
		if (ImGui::IsWindowHovered())
		{
			g_eventMod = eventmod;
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
		g_sceneSize.x = wsize.x;
		g_sceneSize.y = wsize.y;

		ImVec2 wpos = ImGui::GetWindowPos();
		g_scenePosition.x = wpos.x;
		g_scenePosition.y = wpos.y;

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

		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("Play");
			//popup
		}
		
		if (ImGui::BeginPopup("Play"))
		{
			if (!isPlay)
			{
				if (ImGui::Selectable(ICON_FK_PLAY " play "))
				{
					POPEYE_CORE_INFO("play scene");
					isPlay = true;
					g_fileIO->SaveScene();
				}
			}
			else
			{
				if (ImGui::Selectable(ICON_FK_STOP " stop "))
				{
					POPEYE_CORE_INFO("stop play");
					isPlay = false;
					g_fileIO->LoadScene(SceneManager::GetInstance()->currentScenePath);
				}
			}
			ImGui::EndPopup();
		}


		ImGui::EndChild();
	}

	//Tab::Hierarchy
	void Hierarchy::ShowContents()
	{
		scene = SceneManager::GetInstance()->currentScene;
		CheckHover();
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
		{
			ImGui::OpenPopup("GameObject");
			//popup
		}
		if (ImGui::BeginPopup("GameObject"))
		{
			if (ImGui::Selectable("Create GameObject"))
			{
				scene->CreateGameObject("GameObject");
			}

			ImGui::EndPopup();
		}

		if (ImGui::CollapsingHeader(scene->GetName().c_str()))
		{
			int size = scene->gameObjects.size();
			for (int i = 0; i < size; i++)
			{
				if (scene->gameObjects[i] != nullptr)
				{
					if (ImGui::Selectable(scene->gameObjects[i]->GetName().c_str()))
					{
						selectedGameObject = scene->gameObjects[i];
					}
				}
			}
		}

		if (selectedGameObject != nullptr && ImGui::IsKeyDown(261))
		{
			int idToDelete = selectedGameObject->GetID();
			selectedGameObject->DeleteComponent<Camera>();
			selectedGameObject->DeleteComponent<MeshRenderer>();
			selectedGameObject->DeleteComponent<Light>();
			selectedGameObject = nullptr;

			scene->DeleteGameObject(idToDelete);
		}
	}

	//Tab::Inspector
	void Inspector::ShowContents()
	{
		static ImGuiTextFilter filter;
		static bool addcomponentCall = false;

		CheckHover();
		if (selectedGameObject != nullptr) // transform info first
		{
			char nameBuff[128] = "";

			std::string name = selectedGameObject->GetName();
			int size = name.size();

			for (int i = 0; i < 128; i++)
			{
				if (i < size)
					nameBuff[i] = name[i];
				else
					break;
			}

			ImGui::InputText("##name", nameBuff, sizeof(char) * 128);
			if (ImGui::IsKeyDown(257))
			{
				int i = 0;
				std::string name = "";
				for (int i = 0; i < 128; i++)
				{
					if (nameBuff[i] != 0)
						name += nameBuff[i];
					else
						break;
				}
				
				selectedGameObject->SetName(name);
			}

			ImGui::Text("position");
			ImGui::SameLine();
			ImGui::DragFloat3("##pos", (float*)&selectedGameObject->transform.position);

			ImGui::Text("rotation");
			ImGui::SameLine();
			ImGui::DragFloat3("##rot", (float*)&selectedGameObject->transform.rotation);

			ImGui::Text("scale   ");
			ImGui::SameLine();
			ImGui::DragFloat3("##scl", (float*)&selectedGameObject->transform.scale);

			/*Show all Component*/
			int id = selectedGameObject->GetID();
			std::vector<Accessor> accessor = SceneManager::GetInstance()->currentScene->GetAllAddressOfID(id); // TODO::Make copy once.
			bool componentExist = true;
			for (int i = 0; i < accessor.size(); i++)
			{
				if (accessor[i].componentType != "")
				{
					if (accessor[i].componentType == "Camera")
					{
						ShowComponent(selectedGameObject->GetComponent<Camera>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<Camera>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "MeshRenderer")
					{
						ShowComponent(selectedGameObject->GetComponent<MeshRenderer>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<MeshRenderer>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "Light")
					{
						ShowComponent(selectedGameObject->GetComponent<Light>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<Light>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "BoxCollider")
					{
						ShowComponent(selectedGameObject->GetComponent<BoxCollider>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<BoxCollider>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "Rigidbody")
					{
						ShowComponent(selectedGameObject->GetComponent<Rigidbody>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<Rigidbody>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "UIFrame")
					{
						ShowComponent(selectedGameObject->GetComponent<UIFrame>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<UIFrame>();
							componentExist = true;
						}
					}
					else if (accessor[i].componentType == "Text")
					{
						ShowComponent(selectedGameObject->GetComponent<Text>(), componentExist);
						if (!componentExist)
						{
							selectedGameObject->DeleteComponent<Text>();
							componentExist = true;
						}
					}
				}
			}

			/*Add Component*/
			if (ImGui::Button("Add Component", ImVec2(ImGui::GetWindowSize().x, 0.0f)))
				addcomponentCall = true;
			
			if (addcomponentCall)
			{
				filter.Draw("search");
				std::vector<const char*> components = {"Camera", "MeshRenderer", "Light", 
					"BoxCollider", "Rigidbody", "UIFrame" , "Text"};
				for (int i = 0; i < components.size(); i++)
				{
					if (filter.PassFilter(components[i]))
						if (ImGui::Selectable(components[i]))
						{
							selectedGameObject->AddComponentByName(components[i]);
							addcomponentCall = false;
						}
				}

			}
		}
	}


	void Inspector::ShowComponent(Camera& camera, bool& closeBtn)
	{
		const char* camMod[] = { "Perspective", "Otrhomatric" };

		int cameraMod = -1;
		if (camera.mod == Projection::PERSPECTIVE) { cameraMod = 0; }
		if (camera.mod == Projection::ORTHOGRAPHIC) { cameraMod = 1; }

		if (ImGui::CollapsingHeader("Camera", &closeBtn))
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

	void Inspector::ShowComponent(MeshRenderer& meshRenderer, bool& closeBtn)
	{
		if (ImGui::CollapsingHeader("MeshRenderer", &closeBtn))
		{

			ImGui::Text("Mesh");
			ImGui::Selectable("##selectable", false, ImGuiSelectableFlags_SelectOnClick, ImVec2(100.0f, 20.0f));

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsKeyDown(261))
				{
					meshRenderer.meshID = 0;
					meshRenderer.isEmpty = true;
				}
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MODEL"))
				{
					meshRenderer.meshID = *(unsigned int*)payload->Data;
					meshRenderer.isEmpty = false;
				}

				ImGui::EndDragDropTarget();
			}

			ImGui::Text("Material");
			ImGui::Selectable("##selectable", false, ImGuiSelectableFlags_SelectOnClick, ImVec2(100.0f, 20.0f));

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsKeyDown(261))
				{
					meshRenderer.materialID = 0;
				}
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("MATERIAL"))
				{
					meshRenderer.materialID = *(unsigned int*)payload->Data;
				}

				ImGui::EndDragDropTarget();
			}
			if (meshRenderer.materialID != 0 && ImGui::TreeNode("Material"))
			{
				Material& material = g_ResourceManager->materials[meshRenderer.materialID];
				ImGui::Text("texture");
				ImGui::Selectable("##selectable", false, ImGuiSelectableFlags_SelectOnClick, ImVec2(80.0f, 80.0f));
				if (ImGui::IsItemHovered())
				{
					if (ImGui::IsKeyDown(261))
					{
						material.textureID = -1;
					}
				}
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
					{
						int id = *(const int*)payload->Data;

						material.textureID = id;
					}

					ImGui::EndDragDropTarget();
				}
				
				ImDrawList* draw_list = ImGui::GetWindowDrawList();
				const ImVec2 p0 = ImGui::GetItemRectMin();
				const ImVec2 p1 = ImGui::GetItemRectMax();

				ImVec2 imgRectMin = ImVec2(p0.x + 2.5f, p0.y + 2.5f);
				ImVec2 imgRectMax = ImVec2(p1.x - 2.5f, p1.y - 2.5f);

				draw_list->AddImage((ImTextureID)material.textureID, imgRectMin, imgRectMax);
				
				ImGui::ColorEdit3("color", (float*)&material.color);
				
				ImGui::DragFloat("ambient", &material.amb_diff_spec[0]);
				ImGui::DragFloat("diffuse", &material.amb_diff_spec[1]);
				ImGui::DragFloat("specular", &material.amb_diff_spec[2]);
				
				ImGui::DragFloat("shininess",	&material.shininess);
				
				ImGui::TreePop();
			}
		}
	}

	void Inspector::ShowComponent(Light& light, bool& closeBtn)
	{
		const char* lighttype[] = { "Point", "Direction", "Spot" };

		int lightMod = -1;
		LightType type = light.ShowLightType();
		
		if (type == LightType::POINT) { lightMod = 0; }
		else if (type == LightType::DIRECTION) { lightMod = 1; }
		else if (type == LightType::SPOT) { lightMod = 2; }

		if (ImGui::CollapsingHeader("Light", &closeBtn))
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
			
			ImGui::DragFloat("constant",	&light.constant);
			ImGui::DragFloat("linear",		&light.linear);
			ImGui::DragFloat("quadratic",	&light.quadratic);

			ImGui::ColorEdit3("light color", (float*)&light.color);
	
			ImGui::DragFloat("ambient",		&light.ambient);
			ImGui::DragFloat("diffuse",		&light.diffuse);
			ImGui::DragFloat("specular",	&light.specular);
		}
	}

	void Inspector::ShowComponent(BoxCollider& colider, bool& closeBtn)
	{
		if (ImGui::CollapsingHeader("BoxCollider", &closeBtn))
		{
			ImGui::DragFloat("width", &colider.width);
			ImGui::DragFloat("length", &colider.length);
			ImGui::DragFloat("height", &colider.height);
		}
	}

	void Inspector::ShowComponent(Rigidbody& rigidbody, bool& closeBtn)
	{
		if (ImGui::CollapsingHeader("Rigidbody", &closeBtn))
		{
			ImGui::Checkbox("gravity", &rigidbody.gravity);
			ImGui::DragFloat("weight", &rigidbody.weight);
		}
	}

	void Inspector::ShowComponent(UIFrame& uiframe, bool& closeBtn)
	{
		if (ImGui::CollapsingHeader("UIFrame", &closeBtn))
		{
			ImGui::DragFloat2("leftTop", (float*)&uiframe.leftTop);
			ImGui::DragFloat2("leftTop", (float*)&uiframe.rightBot);

			ImGui::Text("texture");
			ImGui::Selectable("##selectable", false, ImGuiSelectableFlags_SelectOnClick, ImVec2(80.0f, 80.0f));
			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsKeyDown(261))
				{
					uiframe.textureID = -1;
				}
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TEXTURE"))
				{
					int id = *(const int*)payload->Data;

					uiframe.textureID = id;
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	void Inspector::ShowComponent(Text& text, bool& closeBtn)
	{
		if (ImGui::CollapsingHeader("Text", &closeBtn))
		{
		}
	}

	//void Inspector::ShowComponent(Button& btn, bool& closeBtn)
	//{
	//	if (ImGui::CollapsingHeader("Button", &closeBtn))
	//	{
	//	}
	//}


	//Tab::Debug
	void Debug::ShowContents()
	{
		CheckHover();
	}

	//Tab::Project TODO :: Try make code shorter
	void Project::ShowContents()
	{
		static ImVec2 fileSize(70.0f, 80.0f);
		CheckHover();
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiContext& g = *GImGui;
		
		float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

		{
			ImGui::BeginChild("Directories", ImVec2(150, 0), true);
			
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

			if (ImGui::TreeNodeEx((void*)(intptr_t)0, flags, ICON_FK_FOLDER" Root"))
			{
				fs::path currpath = fs::current_path() / "Root";

				if (ImGui::IsItemHovered())
				{
					if (ImGui::IsMouseDoubleClicked(0))
					{
						g_fileIO->curr_focused_path = fs::current_path() / "Root";
					}
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIR"))
					{
						std::string str(static_cast<char*>(payload->Data), payload->DataSize);

						fs::path path = str;

						if (fs::equivalent(path, g_fileIO->curr_focused_path))
						{
							g_fileIO->curr_focused_path = (currpath / path.filename());
						}

						fs::rename(path, (currpath / path.filename()).string());
					}
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE"))
					{
						std::string str(static_cast<char*>(payload->Data), payload->DataSize);

						fs::path path = str;

						fs::rename(path, (currpath / path.filename()).string());
					}

					ImGui::EndDragDropTarget();
				}

				ShowDirectories(0, currpath.string());
				ImGui::TreePop();
			}

			ImGui::EndChild();
		}
		ImGui::SameLine();

		{
			ImGui::BeginChild("Files", ImVec2(0, 0), true);

			std::vector<FileData> dirs, files;
			int totalFileNum = g_fileIO->ShowFilesAtDir(dirs, files, g_fileIO->curr_focused_path);

			int i = 0,  fi = 0, di = 0;
			while(i != totalFileNum)
			{
				ImGui::PushID(i);

				ImGui::Selectable("##selectable", false, 0, fileSize);

				FileData filedata;
				if (di < dirs.size())
				{
					filedata = dirs[di];

					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(0))
						{
							g_fileIO->curr_focused_path = filedata.path;
						}
					}
					
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIR"))
						{
							std::string str(static_cast<char*>(payload->Data), payload->DataSize);

							fs::path path = str;

							if (!fs::equivalent(path, filedata.path))
								fs::rename(path, filedata.path / path.filename());
						}
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE"))
						{
							std::string str(static_cast<char*>(payload->Data), payload->DataSize);

							fs::path path = str;

							if (!fs::equivalent(path, filedata.path))
								fs::rename(path, filedata.path / path.filename());
						}

						ImGui::EndDragDropTarget();
					}

					di++;
				}
				else
				{
					filedata = files[fi];
					if (ImGui::IsItemHovered())
					{
						if (ImGui::IsMouseDoubleClicked(0) && filedata.type == FileType::SCENE)
						{
							selectedGameObject = nullptr;
							g_fileIO->LoadScene(filedata.path);
						}
					}
					fi++;
				}

				std::string fileNamestr = filedata.path.filename().string();
				const char* file_name = fileNamestr.c_str();

				const ImVec2 p0 = ImGui::GetItemRectMin();
				const ImVec2 p1 = ImGui::GetItemRectMax();

				ImDrawList* draw_list = ImGui::GetWindowDrawList();

				float centerPosX = (p0.x + p1.x) * 0.5f;

				ImVec2 imgRectMin = ImVec2(centerPosX - 21.0f, p0.y + 2.0f);
				ImVec2 imgRectMax = ImVec2(centerPosX + 25.0f, imgRectMin.y + 50.0f);

				if (filedata.type == FileType::DIR) { draw_list->AddText(g_Icon, 50.0f, imgRectMin, IM_COL32_WHITE, ICON_FOLDER, 0, fileSize.x); }
				else if (filedata.type == FileType::SOURCE) { draw_list->AddText(g_Icon, 50.0f, imgRectMin, IM_COL32_WHITE, ICON_SOURCE, 0, fileSize.x); }
				else if (filedata.type == FileType::IMAGE) { draw_list->AddText(g_Icon, 50.0f, imgRectMin, IM_COL32_WHITE, ICON_IMAGE, 0, fileSize.x); }
				else if (filedata.type == FileType::MODEL) { draw_list->AddText(g_Icon, 50.0f, imgRectMin, IM_COL32_WHITE, ICON_CUBE, 0, fileSize.x); }
				else  { draw_list->AddText(g_Icon, 50.0f, imgRectMin, IM_COL32_WHITE, ICON_TEXT, 0, fileSize.x); }

				ImVec2 textsize = ImGui::CalcTextSize(file_name);
				float text_x_pos = textsize.x < fileSize.x ? centerPosX - (textsize.x * 0.5f) : centerPosX - (fileSize.x * 0.5f);
				ImVec2 text_pos = ImVec2(text_x_pos, imgRectMax.y);

				draw_list->AddText(g.Font, g.FontSize, text_pos, IM_COL32_WHITE, file_name, 0, fileSize.x);

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					std::string path = filedata.path.string();

					ImGui::SetDragDropPayload("FILE", &path[0], path.size(), ImGuiCond_Once);

					ImGui::Text(file_name);

					ImGui::EndDragDropSource();
				}

				ImGui::PopID();

				float last_button_x2 = p1.x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x; // Expected position if next button was on same line
				if (i + 1 < totalFileNum && next_button_x2 < window_visible_x2)
					ImGui::SameLine();

				i++;
			}
			ImGui::EndChild();
		}
	}

	void Project::ShowDirectories(int id, std::string directory)
	{
		ImGui::PushID(id);

		std::vector<DirectoryData> dirs;
		int dircount = g_fileIO->ShowDirAtDir(dirs, directory);
		
		ImGui::AlignTextToFramePadding();
		for (int i = 0; i < dircount; i++)
		{
			ImGui::PushID(i);

			fs::path currDir = dirs[i].path;
			std::string str = ICON_FK_FOLDER + currDir.filename().string();
			const char* filename = str.c_str();
			bool is_open = false;

			ImGui::AlignTextToFramePadding();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			if (!dirs[i].hasSubDir)
			{
				flags |= ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Leaf;
				ImGui::TreeNodeEx((void*)(intptr_t)i, flags, filename);
			}
			else
			{
				is_open = ImGui::TreeNodeEx((void*)(intptr_t)i, flags, filename);
			}

			if (ImGui::IsItemHovered())
			{
				if (ImGui::IsMouseDoubleClicked(0))
				{
					g_fileIO->curr_focused_path = currDir;
				}
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
			{

				std::string path = currDir.string();

				ImGui::SetDragDropPayload("DIR", &path[0], path.size(), ImGuiCond_Once);

				ImGui::Text(filename);

				ImGui::EndDragDropSource();
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DIR"))
				{
					std::string str(static_cast<char*>(payload->Data), payload->DataSize);

					fs::path path = str;


					if (!fs::equivalent(path, currDir))
					{
						if (fs::equivalent(path, g_fileIO->curr_focused_path))
						{
							g_fileIO->curr_focused_path = currDir / path.filename();
						}
						else
						{
							fs::rename(path, currDir / path.filename());
						}
					}
					
				}
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE"))
				{
					std::string str(static_cast<char*>(payload->Data), payload->DataSize);

					fs::path path = str;

					if (!fs::equivalent(path, currDir))
						fs::rename(path, currDir / path.filename());
				}
				ImGui::EndDragDropTarget();
			}

			if (is_open)
			{
				ShowDirectories(i, currDir.string());
				
				ImGui::TreePop();
			}

			ImGui::PopID();
		}

		ImGui::PopID();
		
	}


	void Resource::ShowContents()
	{
		CheckHover();
		if (ImGui::BeginTabBar("ResourceManager"))
		{
			if (ImGui::BeginTabItem("Texture"))
			{
				for (int i = 0; i < g_ResourceManager->textures.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::Selectable("##resource", false, 0, ImVec2(100.0f, 100.0f));
					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					const ImVec2 p0 = ImGui::GetItemRectMin();
					const ImVec2 p1 = ImGui::GetItemRectMax();


					draw_list->AddImage((ImTextureID)g_ResourceManager->textures[i].id, ImVec2(p0.x + 2.0f, p0.y + 2.0f), ImVec2(p1.x - 2.0f, p1.y - 2.0f));


					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						unsigned int id = g_ResourceManager->textures[i].id;

						ImGui::SetDragDropPayload("TEXTURE", &id, sizeof(unsigned int), ImGuiCond_Once);

						ImGui::Text("texture : {0}", id);

						ImGui::EndDragDropSource();
					}


					ImGui::PopID();

					ImGui::SameLine();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Model"))
			{
				for (int i = 0; i < g_ResourceManager->meshes.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::Selectable(g_ResourceManager->meshes[i].name.c_str(), false, 0, ImVec2(100.0f, 100.0f));
					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					const ImVec2 p0 = ImGui::GetItemRectMin();
					const ImVec2 p1 = ImGui::GetItemRectMax();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("MODEL", &i, sizeof(unsigned int), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

					ImGui::PopID();

					ImGui::SameLine();
				}
				
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Material"))
			{
				if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
				{
					ImGui::OpenPopup("Material");
					//popup
				}
				if (ImGui::BeginPopup("Material"))
				{
					if (ImGui::Selectable("Create Material"))
					{
						g_ResourceManager->AddMaterial();
					}

					ImGui::EndPopup();
				}

				for (int i = 1; i < g_ResourceManager->materials.size(); i++)
				{
					ImGui::PushID(i);
					ImGui::Selectable(g_ResourceManager->materials[i].id.c_str(), false, 0, ImVec2(100.0f, 100.0f));
					ImDrawList* draw_list = ImGui::GetWindowDrawList();

					const ImVec2 p0 = ImGui::GetItemRectMin();
					const ImVec2 p1 = ImGui::GetItemRectMax();

					if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
					{
						ImGui::SetDragDropPayload("MATERIAL", &i, sizeof(unsigned int), ImGuiCond_Once);

						ImGui::EndDragDropSource();
					}

					ImGui::PopID();

					ImGui::SameLine();
				}

				ImGui::EndTabItem();
			}
			
			ImGui::InvisibleButton("##empty", ImGui::GetWindowSize());
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("FILE"))
				{
					std::string str(static_cast<char*>(payload->Data), payload->DataSize);

					FileData filedata;
					filedata.path = str;

					if (filedata.path.extension() == ".jpg" || filedata.path.extension() == ".png")
					{
						filedata.type = FileType::IMAGE;
					}

					if (filedata.path.extension() == ".fbx" || filedata.path.extension() == ".obj")
					{
						filedata.type = FileType::MODEL;
					}

					g_fileIO->WriteDataToFile("Resource.dat", "ResourceTable.dat", filedata);
				}
				ImGui::EndDragDropTarget();
			}

			if (ImGui::Button("Set Resource"))
				g_ResourceManager->SetResources();



			ImGui::EndTabBar();
		}

	}

}