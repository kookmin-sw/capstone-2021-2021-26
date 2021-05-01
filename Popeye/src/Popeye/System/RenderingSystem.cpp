#include "RenderingSystem.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/ComponentManager.h"

#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"
#include "../Component/RenderingComponents.h"
#include "../Gizmo.h"

namespace Popeye {
	extern ResourceManager* g_ResourceManager;

	extern glm::vec3 g_Rayorigin;
	extern glm::vec3 g_Raydirection;
	extern bool g_sendRay;

	glm::vec3 g_sceneViewPosition = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 g_sceneViewDirection = glm::vec3(0.0f, 0.0f, 1.0f);


	unsigned int g_GameView;
	unsigned int g_SceneView;

	static Shader g_ScreenShader;

	bool RayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, BoundBox boundbox, glm::mat4 model, float& intersection_distance);

	void RenderingSystem::SystemInit()
	{
		g_ScreenShader.Init("shader/vertexShaderfb.glsl", "shader/fragmentShaderfb.glsl");

		unsigned int rbo;

		glGenFramebuffers(1, &gameViewFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, gameViewFBO);

		//gmae view frame buffer
		glGenTextures(1, &g_GameView);
		glBindTexture(GL_TEXTURE_2D, g_GameView);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_GameView, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//world view frame buffer
		glGenFramebuffers(1, &sceneViewFBO);
		glBindFramebuffer(GL_FRAMEBUFFER, sceneViewFBO);

		glGenTextures(1, &g_SceneView);
		glBindTexture(GL_TEXTURE_2D, g_SceneView);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_SceneView, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// render system should run while not on play...
	void RenderingSystem::SystemRunning()
	{
		if (renderstate == RenderState::RENDERSCENEVIEW)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, sceneViewFBO);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//render
			RenderSceneView();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_DEPTH_TEST);
			g_ScreenShader.use();
			glBindTexture(GL_TEXTURE_2D, g_SceneView);

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			renderstate = RenderState::RENDERGAMEVIEW;
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, gameViewFBO);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//render
			RenderGameView();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_DEPTH_TEST);
			g_ScreenShader.use();
			glBindTexture(GL_TEXTURE_2D, g_GameView);

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			renderstate = RenderState::RENDERSCENEVIEW;
		}		
	}

	void RenderingSystem::RenderGameView()
	{
		static Shader shader; // temp 
		static Camera camera;
		static int init = 0;
		if (!init) { shader.Init("shader/vertexShader.glsl", "shader/fragmentShader.glsl"); init++; }

		int pointLightCount = 0;
		int dirLightCount = 0;
		int spotLightCount = 0;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);

		shader.use();

		shader.setInt("pointlightCount", Light::pointLightCounter);
		shader.setInt("dirlightCount", Light::directionalLightCounter);
		shader.setInt("spotlightCount", Light::spotLightCounter);

		int currentCameraID = SceneManager::GetInstance()->currentScene->mainCameraID;
		GameObject* cameraObject = SceneManager::GetInstance()->currentScene->gameObjects[currentCameraID];
		
		glm::vec3 position = cameraObject->transform.position;
		glm::vec3 rotation = glm::radians(cameraObject->transform.rotation);
		glm::vec3 scale = cameraObject->transform.scale;
		
		camera = SceneManager::GetInstance()->currentScene->GetData<Camera>(currentCameraID);

		view = glm::translate(view, position) * glm::toMat4(glm::quat(rotation));
		view = glm::inverse(view);

		if (camera.mod == Projection::PERSPECTIVE) //Projection :: peripective mod
		{
			projection = glm::perspective(camera.fov, camera.offsetX / camera.offsetY, camera.nearView, camera.farView);
		}
		else if (camera.mod == Projection::ORTHOGRAPHIC) //Projection :: orthographic
		{
			projection = glm::ortho(
				-(camera.width) * 0.5f, (camera.width) * 0.5f,
				-(camera.height) * 0.5f, (camera.height) * 0.5f,
				camera.nearView, camera.farView);
		}

		shader.setVec3("ViewPos", view[3]);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		{
			int id = gameObject->GetID();
			glm::vec3 position	= gameObject->transform.position;
			glm::vec3 rotation	= glm::radians(gameObject->transform.rotation);
			glm::vec3 scale		= gameObject->transform.scale;

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

					shader.setFloat(str + ".cutOff",		glm::cos(glm::radians(light.cutoff)));
					shader.setFloat(str + ".outerCutOff",	glm::cos(glm::radians(light.outercutoff)));

					spotLightCount++;
				}

				shader.setFloat(str + ".constant",	light.constant);
				shader.setFloat(str + ".linear",	light.linear);
				shader.setFloat(str + ".quadratic", light.quadratic);

				shader.setVec3(str + ".ambient",	light.color * light.ambient);
				shader.setVec3(str + ".diffuse",	light.color * light.diffuse);
				shader.setVec3(str + ".specular",	light.color * light.specular);
			}

			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<MeshRenderer>(id))
			{
				MeshRenderer meshrenderer = SceneManager::GetInstance()->currentScene->GetData<MeshRenderer>(id);
				if (!meshrenderer.isEmpty)
				{
					int meshID = meshrenderer.meshID;
					int materialID = meshrenderer.materialID;

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

					shader.setVec3("material.ambient",	material.color * material.amb_diff_spec[0]);
					shader.setVec3("material.diffuse",	material.color * material.amb_diff_spec[1]);
					shader.setVec3("material.specular", material.color * material.amb_diff_spec[2]);

					shader.setFloat("material.shininess", material.shininess);
					
					glBindVertexArray(g_ResourceManager->meshes[meshID].VAO);
					glDrawElements(GL_TRIANGLES, g_ResourceManager->meshes[meshID].indices.size(), GL_UNSIGNED_INT, (void*)0);
					
					glBindVertexArray(0);
				}
			}
		}
	}

	void RenderingSystem::RenderSceneView()
	{
		static Shader shader, gridShader; // temp 
		static Gizmo gizmo;
		static int init = 0;

		static std::vector<glm::vec3> vertices;
		static std::vector<glm::uvec4> indices;

		if (!init) {
			shader.Init("shader/vertexShader.glsl", "shader/fragmentShader.glsl");
			gridShader.Init("shader/gridvert.glsl", "shader/gridfrag.glsl");

			gizmo.Init();
			init++;
		}

		int pointLightCount = 0;
		int dirLightCount = 0;
		int spotLightCount = 0;

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::lookAt(g_sceneViewPosition, g_sceneViewPosition + g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

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
					int meshID = meshrenderer.meshID;
					int materialID = meshrenderer.materialID;

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
					scale *= glm::vec3((boundbox.maxX - boundbox.minX), (boundbox.maxY - boundbox.minY) , (boundbox.maxZ - boundbox.minZ));
					model = glm::mat4(1.0f);
					model = glm::translate(model, position) * glm::toMat4(glm::quat(rotation)) * glm::scale(model, scale);
					shader.setMat4("model", model);
					gizmo.DrawWireCube();

					if (g_sendRay)
					{
						float intersection_distance = 100.0f;
						if (RayOBBIntersection(g_Rayorigin, g_Raydirection, boundbox, model, intersection_distance))
						{
							POPEYE_INFO(g_ResourceManager->meshes[meshID].name);
						}
					}
				}
			}
		}

		{
			gridShader.use();
			gridShader.setMat4("view", view);
			gridShader.setMat4("proj", projection);
			gizmo.DrawGrid();
		}
	}


	bool RayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction, BoundBox boundbox, glm::mat4 model, float& intersection_distance)
	{
		float tMin = 0.0f;
		float tMax = 100000.0f;

		glm::vec3 OBBposition_worldspace(model[3].x, model[3].y, model[3].z);
		glm::vec3 delta = OBBposition_worldspace - ray_origin;

		glm::vec3 minvec = glm::vec3(boundbox.minX, boundbox.minY, boundbox.minZ);
		glm::vec3 maxvec = glm::vec3(boundbox.maxX, boundbox.maxY, boundbox.maxZ);


		glm::vec3 xaxis(model[0].x, model[0].y, model[0].z);
		glm::vec3 yaxis(model[1].x, model[1].y, model[1].z);
		glm::vec3 zaxis(model[2].x, model[2].y, model[2].z);

		float e = glm::dot(xaxis, delta);
		float f = glm::dot(ray_direction, xaxis);

		float t1 = (e + minvec.x) / f;
		float t2 = (e + maxvec.x) / f;

		if (t1 > t2) {
			float w = t1; t1 = t2; t2 = w;
		}
		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin)
			return false;

		
		e = glm::dot(yaxis, delta);
		f = glm::dot(ray_direction, yaxis);

		t1 = (e + minvec.y) / f;
		t2 = (e + maxvec.y) / f;

		if (t1 > t2) {
			float w = t1; t1 = t2; t2 = w;
		}
		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin)
			return false;

		e = glm::dot(zaxis, delta);
		f = glm::dot(ray_direction, zaxis);

		t1 = (e + minvec.z) / f;
		t2 = (e + maxvec.z) / f;

		if (t1 > t2) {
			float w = t1; t1 = t2; t2 = w;
		}
		if (t2 < tMax) tMax = t2;
		if (t1 > tMin) tMin = t1;

		if (tMax < tMin)
			return false;


		return true;
	}
}