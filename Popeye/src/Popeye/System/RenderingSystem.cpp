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
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it

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

			unsigned int rbo2;
			glGenRenderbuffers(1, &rbo2);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo2);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo2); // now actually attach it

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
				Rendering();

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glDisable(GL_DEPTH_TEST);
				screenShader.use();
				glBindTexture(GL_TEXTURE_2D, viewTexture);

				glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				renderstate = RenderState::RENDERSCENEVIEW;
			}

		}

		/***********************game framework***********************/
	}

	void RenderingSystem::Rendering()
	{
		static Shader shader; // temp 
		static Camera camera; 

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		//glm::vec3 viewPos = glm::vec3(0.0f);
		shader.use();
		if (renderstate == RenderState::RENDERGAMEVIEW)
		{
			int currentCameraID = SceneManager::GetInstance()->currentScene->mainCameraID;
			GameObject* cameraObject = SceneManager::GetInstance()->currentScene->gameObjects[currentCameraID];
			glm::vec3 position = cameraObject->transform.position;
			glm::vec3 rotation	= cameraObject->transform.rotation;
			glm::vec3 scale		= cameraObject->transform.scale;
			camera = SceneManager::GetInstance()->currentScene->GetData<Camera>(currentCameraID);

			view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(-1.0f, 0.0f, 0.0f));
			view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0.0f, -1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, -1.0f));
			view = glm::translate(view, -position);

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
			shader.setVec3("viewPos", position);
		}
		else
		{
			glm::vec3 position = g_sceneViewPosition;
			view = glm::lookAt(g_sceneViewPosition, g_sceneViewPosition + g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
			projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
			shader.setVec3("viewPos", position);
		}
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		for (GameObject* gameObject : SceneManager::GetInstance()->currentScene->gameObjects)
		{
			int id = gameObject->GetID();
			glm::vec3 position	= gameObject->transform.position;
			glm::vec3 rotation	= gameObject->transform.rotation;
			glm::vec3 scale		= gameObject->transform.scale;
			
			glm::vec3 vectorValueHanlder = glm::vec3(1.0f);

			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<Light>(id))
			{
				Light light = SceneManager::GetInstance()->currentScene->GetData<Light>(id);

				shader.setVec3("light.position", position);

				vectorValueHanlder = light.color * light.ambient;
				shader.setVec3("light.ambient", vectorValueHanlder);
				
				vectorValueHanlder *= light.diffuse;
				shader.setVec3("light.diffuse", vectorValueHanlder);
				
				vectorValueHanlder *= light.specular;
				shader.setVec3("light.specular", vectorValueHanlder);
			}

			if (SceneManager::GetInstance()->currentScene->CheckIfThereIsData<MeshRenderer>(id))
			{
				MeshRenderer meshrenderer = SceneManager::GetInstance()->currentScene->GetData<MeshRenderer>(id);
				Material material = MeshRenderer::materials[meshrenderer.materialIndex];
				model = glm::mat4(1.0f);
				
				model = glm::translate(model, position);

				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

				model = glm::scale(model, scale);

				shader.setMat4("model", model);

				vectorValueHanlder = material.color * material.ambient;
				shader.setVec3("material.ambient", vectorValueHanlder);
				
				vectorValueHanlder *= material.diffuse;
				shader.setVec3("material.diffuse", vectorValueHanlder);
				
				vectorValueHanlder *= material.specular;
				shader.setVec3("material.specular", vectorValueHanlder);

				shader.setInt("material.shininess", material.shininess);

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