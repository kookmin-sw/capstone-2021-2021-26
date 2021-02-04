#include "pch.h"
#include "RenderingSystem.h"

#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "../Component/RenderingComponents.h"
#include "../Component/Camera.h"

namespace Popeye {
	unsigned int RenderingSystem::viewTexture;
	// render system should run while not on play...

	void RenderingSystem::SystemRunning()
	{
		static int state = 0; //temp state;
		static Shader shader; // also temp 
		static Texture texture; // **

		static Shader screenShader("shader/vertexShaderfb.glsl", "shader/fragmentShaderfb.glsl");

		static float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
		};

		if (state == 0)
		{
			/****************************************frame buffer****************************************************/
			/********************************************************************************************************/
			glGenFramebuffers(1, &FBO);
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			// create a color attachment texture
			glGenTextures(1, &viewTexture);
			glBindTexture(GL_TEXTURE_2D, viewTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1200, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, viewTexture, 0);
			// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
			unsigned int rbo;
			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 1200, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
			// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			
			unsigned int sceneVBO;
			glGenBuffers(1, &sceneVBO);
			glBindBuffer(GL_ARRAY_BUFFER, sceneVBO);

			glGenVertexArrays(1, &sceneVAO);
			glBindVertexArray(sceneVAO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
			glEnableVertexAttribArray(1);
			
			/********************************************************************************************************/

			for (int i = 0; i < MeshRenderer::meshes.size(); i++)
			{
				unsigned int tVBO;
				glGenBuffers(1, &tVBO);
				glBindBuffer(GL_ARRAY_BUFFER, tVBO);

				unsigned int tVAO;
				glGenVertexArrays(1, &tVAO);
				glBindVertexArray(tVAO);

				unsigned int tEBO;
				//glGenBuffers(1, &tEBO);
				/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);*/

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
			texture.InitTexture("texture/test.jpg");

			screenShader.use();
			screenShader.setInt("screenTexture", 0);
		}
		else if (state == 1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);
			for (int id : SceneManager::GetInstance()->currentScene->gameobject_IDs)
			{
				glm::vec3 position = Transform::position[id];
				glm::vec3 rotation = Transform::rotation[id];
				glm::vec3 scale = Transform::scale[id];
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
							- (camera.size / 2.0f),  (camera.size / 2.0f),
							- (camera.size / 2.0f),  (camera.size / 2.0f),
							camera.nearView , camera.farView);
					}

					shader.setMat4("view", view);
					shader.setMat4("projection", projection);
				}

				if (MeshRenderer::renderables.find(id) != MeshRenderer::renderables.end()) //Rendering
				{

					texture.drawTexture();
					//shader.use();
					
					glm::mat4 model = glm::mat4(1.0f);
					model = glm::translate(model, position);
					
					model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					model = glm::scale(model, scale);

					shader.setMat4("model", model);


					glBindVertexArray(VAOs[MeshRenderer::renderables[id].first]);
					glDrawArrays(GL_TRIANGLES, 0, 36);
					//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			}
			// now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
			// clear all relevant buffers

			screenShader.use();
			glBindVertexArray(sceneVAO);
			glBindTexture(GL_TEXTURE_2D, viewTexture);	// use the color attachment texture as the texture of the quad plane
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
			glClear(GL_COLOR_BUFFER_BIT);

		}

		/***********************game framework***********************/
		//init -> initializing

		// onUpdate -> main loop

		// onexit -> end
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