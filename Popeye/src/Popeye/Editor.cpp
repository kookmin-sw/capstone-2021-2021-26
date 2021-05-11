#include "Editor.h"
#include "./Manager/SceneManager.h"
#include "./Manager/ResourceManager.h"
#include "./Manager/ComponentManager.h"

#include "./Scene/Scene.h"
#include "./Scene/GameObject.h"
#include "./Component/RenderingComponents.h"

namespace Popeye {
	
	//-------------------------------------------
	// Global variables
	//-------------------------------------------
	extern ResourceManager* g_ResourceManager;;
	
	extern GameObject* selectedGameObject;

	extern bool g_sendRay;

	extern bool g_draggin;

	extern glm::vec2 g_MousePos;

	extern glm::vec2 g_scenePosition;

	extern glm::vec2 g_sceneSize;

	static Shader screenShader;

	unsigned int g_SceneView;

	//-------------------------------------------
	// Class Editor
	//-------------------------------------------
	Editor::Editor() 
	{
		editorCamPos = glm::vec3(2.0f, 2.0f, 2.0f);
		editorCamDir = glm::vec3(0.0f, 0.0f, 1.0f);
	}
	Editor::~Editor() {}

	void Editor::Init()
	{
		screenShader.Init("shader/vertexShaderfb.glsl", "shader/fragmentShaderfb.glsl");
		gizmo.Init();

		unsigned int rbo;

		glGenFramebuffers(1, &editorFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, editorFBO);

		glGenTextures(1, &g_SceneView);
		glBindTexture(GL_TEXTURE_2D, g_SceneView);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_SceneView, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void Editor::OnRun()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, editorFBO);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//render
		RenderView();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glDisable(GL_DEPTH_TEST);
		screenShader.use();
		glBindTexture(GL_TEXTURE_2D, g_SceneView);

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Editor::RenderView()
	{
		static Shader shader, gizmoShader; // temp 
		static int init = 0;
		std::vector<GameObject*> selectable_gameobjects;
		std::vector<BoundBox> sized_boundbox;


		if (!init) {
			shader.Init("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
			gizmoShader.Init("shader/gridvert.glsl", "shader/gridfrag.glsl");
			init++;
		}

		unsigned int pointLightCount = 0;
		unsigned int dirLightCount = 0;
		unsigned int spotLightCount = 0;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(editorCamPos, editorCamPos + editorCamDir, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(45.0f, 1200.0f / 600.0f, 0.1f, 100.0f);

		shader.use();

		shader.setInt("pointlightCount", Light::pointLightCounter);
		shader.setInt("dirlightCount", Light::directionalLightCounter);
		shader.setInt("spotlightCount", Light::spotLightCounter);

		shader.setVec3("ViewPos", view[3]);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		{
			int id = gameObject->GetID();
			glm::vec3 position = gameObject->transform.position;
			glm::vec3 rotation = glm::radians(gameObject->transform.rotation);
			glm::vec3 scale = gameObject->transform.scale;

			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<Light>(id))
			{
				Light light = SceneManager::GetInstance()->currentScene->GetData<Light>(id);
				LightType type = light.ShowLightType();
				std::string str;

				if (type == LightType::POINT)
				{
					str = "pointLights[" + std::to_string(pointLightCount) + "]";

					shader.setVec3(str + ".position", position);

					pointLightCount++;
				}
				else if (type == LightType::DIRECTION)
				{
					str = "dirLights[" + std::to_string(dirLightCount) + "]";

					glm::vec3 direction = glm::vec3(0.0f);

					direction.x = cos(-90.0f - rotation.y) * cos(rotation.x);
					direction.y = sin(rotation.x);
					direction.z = sin(-90.0f - rotation.y) * cos(rotation.x);

					shader.setVec3(str + ".direction", glm::normalize(direction));

					dirLightCount++;
				}
				else if (type == LightType::SPOT)
				{
					str = "spotLights[" + std::to_string(spotLightCount) + "]";

					glm::vec3 direction = glm::vec3(0.0f);

					direction.x = cos(-90.0f - rotation.y) * cos(rotation.x);
					direction.y = sin(rotation.x);
					direction.z = sin(-90.0f - rotation.y) * cos(rotation.x);

					shader.setVec3(str + ".direction", glm::normalize(direction));

					shader.setVec3(str + ".position", position);

					shader.setFloat(str + ".cutOff", glm::cos(glm::radians(light.cutoff)));
					shader.setFloat(str + ".outerCutOff", glm::cos(glm::radians(light.outercutoff)));

					spotLightCount++;
				}

				shader.setFloat(str + ".constant", light.constant);
				shader.setFloat(str + ".linear", light.linear);
				shader.setFloat(str + ".quadratic", light.quadratic);

				shader.setVec3(str + ".ambient", light.color * light.ambient);
				shader.setVec3(str + ".diffuse", light.color * light.diffuse);
				shader.setVec3(str + ".specular", light.color * light.specular);
			}

			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<MeshRenderer>(id))
			{
				MeshRenderer meshrenderer = SceneManager::GetInstance()->currentScene->GetData<MeshRenderer>(id);
				if (!meshrenderer.isEmpty)
				{
					unsigned int meshID = meshrenderer.meshID;
					unsigned int materialID = meshrenderer.materialID;

					Material material = g_ResourceManager->materials[materialID];

					model = glm::mat4(1.0f);
					model = glm::translate(model, position) * glm::toMat4(glm::quat(rotation)) * glm::scale(model, scale);

					shader.setMat4("model", model);

					if (material.textureID == -1)
					{
						shader.setBool("material.text", false);
					}
					else
					{
						shader.setBool("material.text", true);
						shader.setInt("material.texture", 1);

						glActiveTexture(GL_TEXTURE1);
						glBindTexture(GL_TEXTURE_2D, material.textureID);
					}

					shader.setVec3("material.ambient", material.color * material.amb_diff_spec[0]);
					shader.setVec3("material.diffuse", material.color * material.amb_diff_spec[1]);
					shader.setVec3("material.specular", material.color * material.amb_diff_spec[2]);

					shader.setFloat("material.shininess", material.shininess);

					g_ResourceManager->meshes[meshID].DrawMesh();

					//temp
					BoundBox boundbox = g_ResourceManager->meshes[meshID].boundbox;
					glm::vec3 boxscale = boundbox.maxPos - boundbox.minPos;

					boundbox.maxPos *= scale;
					boundbox.minPos *= scale;

					selectable_gameobjects.push_back(gameObject);
					sized_boundbox.push_back(boundbox);
				}
			}
		}


		{	//gizmo
			gizmoShader.use();
			gizmoShader.setMat4("view", view);
			gizmoShader.setMat4("proj", projection);

			model = glm::mat4(1.0f);
			gizmoShader.setMat4("model", model);
			gizmo.DrawGrid();

			int size = selectable_gameobjects.size();
			for (int i = 0; i < size; i++)
			{
				Transform bound_trans = selectable_gameobjects[i]->transform;
				bound_trans.rotation = glm::radians(selectable_gameobjects[i]->transform.rotation);

				glm::vec3 boxscale = (sized_boundbox[i].maxPos - sized_boundbox[i].minPos);

				model = glm::mat4(1.0f);
				model = glm::translate(model, bound_trans.position) * glm::toMat4(glm::quat(bound_trans.rotation)) * glm::scale(model, boxscale);

				gizmoShader.setMat4("model", model);

				gizmo.DrawWireCube();

				if (g_sendRay)
				{
					glm::vec3 screenToMouseStartPos, screenToMouseDir;
					ScreenToWorldPos(g_MousePos, view, projection, screenToMouseStartPos, screenToMouseDir);

					sized_boundbox[i].minPos *= bound_trans.scale;
					sized_boundbox[i].maxPos *= bound_trans.scale;
					if (RayOBBIntersection(screenToMouseStartPos, screenToMouseDir, sized_boundbox[i], model))
					{
						selectedGameObject = selectable_gameobjects[i];
						POPEYE_CORE_INFO("{0}", selectedGameObject->GetName());
					}
				}
			}

			if (selectedGameObject != nullptr)
			{
				glm::vec3 screenToMouseStartPos(0.0f), screenToMouseDir(0.0f);
				if (g_sendRay)
				{
					ScreenToWorldPos(g_MousePos, view, projection, screenToMouseStartPos, screenToMouseDir);
				}
				model = EditTransform(selectedGameObject, screenToMouseStartPos, screenToMouseDir);
				
				gizmoShader.setMat4("model", model);
				gizmo.DrawAxis();
			}
		}
	}

	glm::mat4 Editor::EditTransform(GameObject* selected_gameObject, glm::vec3 mouse_pos, glm::vec3 mouse_dir)
	{
		static int axis = 0;
		static bool axisSelected = false;
		
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 pos = selectedGameObject->transform.position;
		glm::vec3 rot = glm::radians(selectedGameObject->transform.rotation);
		glm::vec3 scl = selectedGameObject->transform.scale;

		model = glm::translate(model, pos) * glm::toMat4(glm::quat(rot));

		if (g_sendRay)
		{
			// check x axis
			if (!axisSelected)
			{
				BoundBox axisboundbox;
				axisboundbox.minPos = glm::vec3(0.1f, -1.0f, -1.0f);
				axisboundbox.maxPos = glm::vec3(1.0f, 0.1f, 0.1f);

				axisSelected = RayOBBIntersection(mouse_pos, mouse_dir, axisboundbox, model);
				axis = 0;
			}

			// check y axis
			if (!axisSelected)
			{
				BoundBox axisboundbox;
				axisboundbox.minPos = glm::vec3(-0.1f, 0.1f, -0.1f);
				axisboundbox.maxPos = glm::vec3(0.1f, 1.0f, 0.1f);

				axisSelected = RayOBBIntersection(mouse_pos, mouse_dir, axisboundbox, model);
				axis = 1;
			}

			// check z axis
			if (!axisSelected)
			{
				BoundBox axisboundbox;
				axisboundbox.minPos = glm::vec3(-0.1f, -0.1f, 0.1f);
				axisboundbox.maxPos = glm::vec3(0.1f, 0.1f, 1.0f);

				axisSelected = RayOBBIntersection(mouse_pos, mouse_dir, axisboundbox, model);
				axis = 2;
			}

			if (axisSelected && g_draggin)
			{
				// position
				/*float distance = glm::distance(mouse_pos, pos);
				glm::vec3 cal = ShortestPoint(mouse_pos + distance * mouse_dir, pos, pos + glm::vec3(model[axis]));
				selectedGameObject->transform.position = cal;*/

				// rotation
				/*glm::vec3 cal = HitPointOfSphere(mouse_pos, mouse_dir, pos);
				glm::vec3 sphere_cord = glm::normalize(cal - pos);
				
				float phi = glm::degrees(glm::atan(sphere_cord.x, sphere_cord.z));
				float theta = glm::degrees(glm::acos(sphere_cord.y));

				selectedGameObject->transform.rotation.y = phi;
				selectedGameObject->transform.rotation.x = theta - 90.0f;*/
				
				// scale
				/*float distance = glm::distance(mouse_pos, pos);
				glm::vec3 cal = ShortestPoint(mouse_pos + distance * mouse_dir, pos, pos + glm::vec3(model[axis]));
				selectedGameObject->transform.scale[axis] = cal[axis];*/

			}
		}

		if (!g_draggin)
			axisSelected = false;

		return model;
	}

	bool Editor::RayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_end, BoundBox boundbox, const glm::mat4& model)
	{
		float tMin = 0.0f;
		float tMax = 100000.0f;
		glm::vec3 rayPos(ray_origin);
		glm::vec3 rayDir(ray_end);

		glm::vec3 pos(model[3].x, model[3].y, model[3].z);
		glm::vec3 delta = pos - rayPos;

		glm::vec3 minvec(boundbox.minPos);
		glm::vec3 maxvec(boundbox.maxPos);

		{
			glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);

			float e = glm::dot(xaxis, delta);
			float f = glm::dot(rayDir, xaxis);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e + minvec.x) / f;
				float t2 = (e + maxvec.x) / f;

				if (t1 > t2) {
					float w = t1; t1 = t2; t2 = w;
				}
				if (t2 < tMax) tMax = t2;
				if (t1 > tMin) tMin = t1;

				if (tMax < tMin)
					return false;
			}
			else {
				if (-e + minvec.x > 0.0f || -e + maxvec.x < 0.0f)
					return false;

			}
		}

		{
			glm::vec3 yaxis(model[1].x, model[1].y, model[1].z);

			float e = glm::dot(yaxis, delta);
			float f = glm::dot(rayDir, yaxis);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e + minvec.y) / f;
				float t2 = (e + maxvec.y) / f;

				if (t1 > t2) {
					float w = t1; t1 = t2; t2 = w;
				}
				if (t2 < tMax) tMax = t2;
				if (t1 > tMin) tMin = t1;

				if (tMax < tMin)
					return false;
			}
			else {
				if (-e + minvec.y > 0.0f || -e + maxvec.y < 0.0f)
					return false;

			}
		}

		{
			glm::vec3 zaxis(model[2].x, model[2].y, model[2].z);

			float e = glm::dot(zaxis, delta);
			float f = glm::dot(rayDir, zaxis);

			if (fabs(f) > 0.001f)
			{
				float t1 = (e + minvec.z) / f;
				float t2 = (e + maxvec.z) / f;

				if (t1 > t2) {
					float w = t1; t1 = t2; t2 = w;
				}
				if (t2 < tMax) tMax = t2;
				if (t1 > tMin) tMin = t1;

				if (tMax < tMin)
					return false;
			}
			else {
				if (-e + minvec.z > 0.0f || -e + maxvec.z < 0.0f)
					return false;

			}
		}

		glm::vec3 hit_pos = rayPos + tMin * rayDir;

		if (tMin > 0)
			return true;

		return true;
	}

	void Editor::ScreenToWorldPos(glm::vec2 mousePos, const glm::mat4& view, const glm::mat4& proj, glm::vec3& screenToMouseStartPos, glm::vec3& screenToMouseDir)
	{

		glm::vec2 mouseCoord = glm::vec2(0.0f, 0.0f);
		glm::vec2 screepos = g_scenePosition;
		glm::vec2 screensize = g_sceneSize;
		mouseCoord.x = ((float)mousePos.x - screepos.x) / (screensize.x * 0.5f) - 1.0f;
		mouseCoord.y = ((float)mousePos.y - screepos.y) / (screensize.y * 0.5f) - 1.0f;

		glm::mat4 inverseProj = glm::inverse(proj);
		glm::mat4 inverseView = glm::inverse(view);

		glm::vec4 screenPos = glm::vec4(mouseCoord.x, -mouseCoord.y, 1.0f, 1.0f);
		glm::vec4 lRayEnd_NDC(mouseCoord.x, -mouseCoord.y, 0.0f, 1.0f);


		glm::vec4 lRayStart_camera = inverseProj * screenPos;    lRayStart_camera /= lRayStart_camera.w;
		glm::vec4 lRayStart_world = inverseView * lRayStart_camera; lRayStart_world /= lRayStart_world.w;
		glm::vec4 lRayEnd_camera = inverseProj * lRayEnd_NDC;      lRayEnd_camera /= lRayEnd_camera.w;
		glm::vec4 lRayEnd_world = inverseView * lRayEnd_camera;   lRayEnd_world /= lRayEnd_world.w;

		glm::vec3 lRayDir_world(lRayEnd_world - lRayStart_world);
		lRayDir_world = glm::normalize(lRayDir_world);

		screenToMouseStartPos = glm::vec3(lRayStart_world);
		screenToMouseDir = glm::normalize(glm::vec3(lRayDir_world));
	}

	glm::vec3 Editor::ShortestPoint(glm::vec3 ray_end, glm::vec3 startPos, glm::vec3 endPos)
	{
		glm::vec3 p;

		float t = -glm::dot((startPos - ray_end), (endPos - startPos));

		p = startPos + (endPos - startPos) * t;

		return p;
	}

	glm::vec3 Editor::HitPointOfSphere(glm::vec3 ray_origin, glm::vec3 ray_end, glm::vec3 pos)
	{
		glm::vec3 p;
		
		float t_ca = glm::dot((ray_end), (pos - ray_origin));
		float l = glm::distance2(pos , ray_origin);
		float d2 = l - t_ca * t_ca;
		if (d2 > 1.0f)
		{
			float max_t = glm::sqrt(l - 1.0f);
			p = ray_origin + max_t * (ray_end);
			return p;
		}
		
		float t_hc = glm::sqrt(1.0f - d2);
		float t = t_ca + t_hc;


		p = ray_origin + t * (ray_end);

		return p;
	}


	//-------------------------------------------
	// Class Gizmo
	//-------------------------------------------
	Gizmo::Gizmo() {}
	Gizmo::~Gizmo() {}

	void Gizmo::Init()
	{
		//init grid
		for (int j = -5; j <= 5; j++) {
			for (int i = -5; i <= 5; i++) {
				float x = (float)i;
				float y = 0;
				float z = (float)j;
				grid.vertices.push_back(glm::vec3(x, y, z));
			}
		}

		for (int j = 0; j < 10; j++) {
			for (int i = 0; i < 10; i++) {

				int row1 = j * (10 + 1);
				int row2 = (j + 1) * (10 + 1);

				grid.indices.push_back(glm::uvec2(row1 + i, row1 + i + 1));
				grid.indices.push_back(glm::uvec2(row1 + i + 1, row2 + i + 1));
				grid.indices.push_back(glm::uvec2(row2 + i + 1, row2 + i));
				grid.indices.push_back(glm::uvec2(row2 + i, row1 + i));

			}
		}

		glGenVertexArrays(1, &grid.VAO);
		glBindVertexArray(grid.VAO);

		unsigned int gridvbo;
		glGenBuffers(1, &gridvbo);
		glBindBuffer(GL_ARRAY_BUFFER, gridvbo);
		glBufferData(GL_ARRAY_BUFFER, grid.vertices.size() * sizeof(glm::vec3), glm::value_ptr(grid.vertices[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		unsigned int gridibo;
		glGenBuffers(1, &gridibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gridibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid.indices.size() * sizeof(glm::uvec2), glm::value_ptr(grid.indices[0]), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		wirecube.vertices ={
			//bottom
			glm::vec3(-0.5f, 0.0f, -0.5f),
			glm::vec3(-0.5f, 0.0f, 0.5f),
			glm::vec3(0.5f, 0.0f, 0.5f),
			glm::vec3(0.5f, 0.0f, -0.5f),
			
			//up
			glm::vec3(-0.5f, 1.0f, -0.5f),
			glm::vec3(-0.5f, 1.0f, 0.5f),
			glm::vec3(0.5f, 1.0f, 0.5f),
			glm::vec3(0.5f, 1.0f, -0.5f),
		};

		wirecube.indices = {
			glm::uvec2(0, 1),
			glm::uvec2(1, 2),
			glm::uvec2(2, 3),
			glm::uvec2(3, 0),

			glm::uvec2(0, 4),
			glm::uvec2(1, 5),
			glm::uvec2(2, 6),
			glm::uvec2(3, 7),

			glm::uvec2(4, 5),
			glm::uvec2(5, 6),
			glm::uvec2(6, 7),
			glm::uvec2(7, 4)
		};

		glGenVertexArrays(1, &wirecube.VAO);
		glBindVertexArray(wirecube.VAO);

		unsigned int cubevbo;
		glGenBuffers(1, &cubevbo);
		glBindBuffer(GL_ARRAY_BUFFER, cubevbo);
		glBufferData(GL_ARRAY_BUFFER, wirecube.vertices.size() * sizeof(glm::vec3), glm::value_ptr(wirecube.vertices[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		unsigned int cubeibo;
		glGenBuffers(1, &cubeibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, wirecube.indices.size() * sizeof(glm::uvec2), glm::value_ptr(wirecube.indices[0]), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		
		axis.vertices = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 1.0f)
		};

		axis.indices = {
			glm::uvec2(0, 1),
			glm::uvec2(0, 2),
			glm::uvec2(0, 3),
			glm::uvec2(0, 1)
		};

		glGenVertexArrays(1, &axis.VAO);
		glBindVertexArray(axis.VAO);

		unsigned int axisvbo;
		glGenBuffers(1, &axisvbo);
		glBindBuffer(GL_ARRAY_BUFFER, axisvbo);
		glBufferData(GL_ARRAY_BUFFER, axis.vertices.size() * sizeof(glm::vec3), glm::value_ptr(axis.vertices[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

		unsigned int axisibo;
		glGenBuffers(1, &axisibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, axisibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, axis.indices.size() * sizeof(glm::uvec2), glm::value_ptr(axis.indices[0]), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	void Gizmo::DrawAxis()
	{
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(axis.VAO);
		glLineWidth(5.0f);
		glDrawElements(GL_LINES, axis.indices.size() * 2, GL_UNSIGNED_INT, NULL);
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(0);
	}

	void Gizmo::DrawGrid()
	{
		glBindVertexArray(grid.VAO);
		glLineWidth(1.5f);
		glDrawElements(GL_LINES, grid.indices.size() * 2, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	void Gizmo::DrawWireCube()
	{
		glBindVertexArray(wirecube.VAO);
		glLineWidth(0.5f);
		glDrawElements(GL_LINES, wirecube.indices.size() * 2, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}