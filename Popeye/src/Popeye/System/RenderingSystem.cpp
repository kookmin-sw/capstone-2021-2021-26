#include "pch.h"
#include "RenderingSystem.h"
#include "../Scene/SceneManger.h"
#include "../Scene/Scene.h"
#include "../Scene/GameObject.h"
#include "../Component/ComponentManager.h"
#include "../Component/RenderingComponents.h"

namespace Popeye {
	glm::vec3 g_sceneViewPosition = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 g_sceneViewDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	unsigned int RenderingSystem::viewTexture;
	unsigned int RenderingSystem::worldTexture;

	// render system should run while not on play...

	void RenderingSystem::SystemRunning()
	{
		static int state = 0; //temp state;

		static Shader screenShader("shader/vertexShaderfb.glsl", "shader/fragmentShaderfb.glsl");

		if (state == 0)
		{
			renderstate = RenderState::RENDERSCENEVIEW;
			/****************************************frame buffer****************************************************/
			/********************************************************************************************************/
			
			//game view frame buffer
			glGenFramebuffers(1, &gameViewFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, gameViewFBO);
			
			// create a color attachment texture
			glGenTextures(1, &viewTexture);
			glBindTexture(GL_TEXTURE_2D, viewTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewTexture, 0);

			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//world view frame buffer
			glGenFramebuffers(1, &worldViewFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, worldViewFBO);

			glGenTextures(1, &worldTexture);
			glBindTexture(GL_TEXTURE_2D, worldTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, worldTexture, 0);

			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600); 
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			state = 1;
			/********************************************************************************************************/

			screenShader.use();
			screenShader.setInt("screenTexture", 0);
		}
		else if (state == 1)
		{
			glViewport(0, 0, 1200, 600);

			if (renderstate == RenderState::RENDERSCENEVIEW)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, worldViewFBO);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//render
				Rendering();
				
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glDisable(GL_DEPTH_TEST);
				screenShader.use();
				glBindTexture(GL_TEXTURE_2D, worldTexture);

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				renderstate = RenderState::RENDERGAMEVIEW;
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, gameViewFBO);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//render
				Rendering();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glDisable(GL_DEPTH_TEST);
				screenShader.use();
				glBindTexture(GL_TEXTURE_2D, viewTexture);

				glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				renderstate = RenderState::RENDERSCENEVIEW;
			}

		}
	}

	void RenderingSystem::Rendering()
	{
		static Shader shader; // temp 
		static Camera camera; 

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

		//camera
		if (renderstate == RenderState::RENDERGAMEVIEW)
		{
			int currentCameraID = SceneManager::GetInstance()->currentScene->mainCameraID;
			GameObject* cameraObject = SceneManager::GetInstance()->currentScene->gameObjects[currentCameraID];
			glm::vec3 position = cameraObject->transform.position;
			glm::vec3 rotation	= glm::radians(cameraObject->transform.rotation);
			glm::vec3 scale		= cameraObject->transform.scale;
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
		}
		else
		{
			view = glm::lookAt(g_sceneViewPosition, g_sceneViewPosition + g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
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
				Material material = MeshRenderer::materials[meshrenderer.materialIndex];
				
				model = glm::mat4(1.0f);
				model = glm::translate(model, position) * glm::toMat4(glm::quat(rotation)) * glm::scale(model, scale);

				shader.setMat4("model", model);

				shader.setVec3("material.ambient",	material.color * material.amb_diff_spec[0]);
				shader.setVec3("material.diffuse",	material.color * material.amb_diff_spec[1]);
				shader.setVec3("material.specular", material.color * material.amb_diff_spec[2]);

				shader.setFloat("material.shininess", material.shininess);

				glBindVertexArray(MeshRenderer::meshes[meshrenderer.meshIndex].VAO);
				glDrawArrays(GL_TRIANGLES, 0, 36);
				//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			}
		}
	}

	// On play -> init -> update -> on exit....
	void RenderingSystem::Init()
	{
	}

	void RenderingSystem::OnUpdate()
	{
	}

	void RenderingSystem::OnExit()
	{
	}



}