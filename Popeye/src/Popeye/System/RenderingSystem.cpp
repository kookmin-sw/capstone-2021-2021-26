#include "pch.h"
#include "RenderingSystem.h"

#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "../Component/RenderingComponents.h"
#include "../Component/Camera.h"

namespace Popeye {
	glm::vec3 g_sceneViewPosition = glm::vec3(2.0f, 2.0f, 2.0f);
	glm::vec3 g_sceneViewDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	unsigned int RenderingSystem::viewTexture;
	unsigned int RenderingSystem::worldTexture;

	// render system should run while not on play...

	void RenderingSystem::SystemRunning()
	{
		static RenderState renderstate;
		static int state = 0; //temp state;

		static Shader screenShader("shader/vertexShaderfb.glsl", "shader/fragmentShaderfb.glsl");

		if (state == 0)
		{
			/****************************************frame buffer****************************************************/
			/********************************************************************************************************/
			
			//game view frame buffer
			glGenFramebuffers(1, &FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			
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

			/********************************************************************************************************/

			for (int i = 0; i < MeshRenderer::meshes.size(); i++)
			{
				unsigned int tVBO;
				glGenBuffers(1, &tVBO);
				glBindBuffer(GL_ARRAY_BUFFER, tVBO);

				unsigned int tVAO;
				glGenVertexArrays(1, &tVAO);
				glBindVertexArray(tVAO);

				/*unsigned int tEBO;
				glGenBuffers(1, &tEBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);*/

				glBufferData(GL_ARRAY_BUFFER, MeshRenderer::meshes[i].vertsize, &MeshRenderer::meshes[i].vertices[0], GL_STATIC_DRAW);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshRenderer::meshes[i].indsize, &MeshRenderer::meshes[i].indicies[0], GL_STATIC_DRAW);

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);*/
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);

				VAOs.push_back(tVAO);
			}
			state = 1;

			screenShader.use();
			screenShader.setInt("screenTexture", 0);
		}
		else if (state == 1)
		{
			glViewport(0, 0, 1200, 600);

			if (renderstate == RenderState::RENDERWORLD)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, worldViewFBO);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//render
				Render(renderstate);
				
				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glDisable(GL_DEPTH_TEST);
				screenShader.use();
				glBindTexture(GL_TEXTURE_2D, worldTexture);

				glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
				glClear(GL_COLOR_BUFFER_BIT);

				renderstate = RenderState::RENDERCAMERAVIEW;
			}
			else
			{
				glBindFramebuffer(GL_FRAMEBUFFER, FBO);
				glEnable(GL_DEPTH_TEST);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				//render
				Render(renderstate);

				glBindFramebuffer(GL_FRAMEBUFFER, 0);

				glDisable(GL_DEPTH_TEST);
				screenShader.use();
				glBindTexture(GL_TEXTURE_2D, viewTexture);

				glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
				glClear(GL_COLOR_BUFFER_BIT);

				renderstate = RenderState::RENDERWORLD;
			}

		}

		/***********************game framework***********************/
		//init -> initializing

		// onUpdate -> main loop

		// onexit -> end
	}

	void RenderingSystem::Render(RenderState& state)
	{
		static Shader shader; // temp 
		static Shader shader2; // also temp 
		static Texture texture; // ..
		static int  init = 0;
		if (init == 0) {
			texture.InitTexture("texture/test.jpg");
			init = 1;
		}
		static glm::mat4 worldView = glm::mat4(1.0f);
		static glm::mat4 worldProjection = glm::mat4(1.0f);
		//static glm::vec3 worldPos = { 5.0f, 5.0f, 5.0f };

		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		for (int id : SceneManager::GetInstance()->currentScene->gameobject_IDs)
		{
			glm::vec3 position = Transform::position[id];
			glm::vec3 rotation = Transform::rotation[id];
			glm::vec3 scale = Transform::scale[id];
			if (state == RenderState::RENDERCAMERAVIEW)
			{
				shader.use();
				if (Camera::cameras.find(id) != Camera::cameras.end()) //Camera
				{
					Cam camera = Camera::cameras[id];
					view = glm::lookAt(position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //View

					if (camera.mod == Projection::PERSPECTIVE) //Projection :: peripective mod
					{
						projection = glm::perspective(camera.fov, camera.offsetX / camera.offsetY, camera.nearView, camera.farView);
					}
					else if (camera.mod == Projection::ORTHOGRAPHIC) //Projection :: orthographic
					{
						projection = glm::ortho(
							-(camera.size / 2.0f), (camera.size / 2.0f),
							-(camera.size / 2.0f), (camera.size / 2.0f),
							camera.nearView, camera.farView);
					}

					shader.setMat4("view", view);
					shader.setMat4("projection", projection);
				}
			}
			else
			{
				shader2.use();
				worldView = glm::lookAt(g_sceneViewPosition, g_sceneViewPosition + g_sceneViewDirection, glm::vec3(0.0f, 1.0f, 0.0f));
				worldProjection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
				shader2.setMat4("view", worldView);
				shader2.setMat4("projection", worldProjection);
			}

			if (MeshRenderer::renderables.find(id) != MeshRenderer::renderables.end()) //Rendering
			{

				texture.drawTexture();

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, position);

				model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

				model = glm::scale(model, scale);

				if (state == RenderState::RENDERCAMERAVIEW)
				{
					shader.setMat4("model", model);
				}
				else
				{
					shader2.setMat4("model", model);
				}

				glBindVertexArray(VAOs[MeshRenderer::renderables[id].first]);
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