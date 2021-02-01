#include "pch.h"
#include "RenderingSystem.h"

#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "../Component/RenderingComponents.h"

namespace Popeye {
	
	// render system should run while not on play...
	void RenderingSystem::SystemRunning()
	{
		static int state = 0; //temp state;
		static Shader shader;
		static Shader Screenshader("shader/vertexShaderfb.glsl","shader/fragmentShaderfb.glsl");
		static unsigned int textureColorbuffer;
		static Texture texture;
		if (state == 0)
		{
			glEnable(GL_DEPTH_TEST);
			for (int i = 0; i < MeshRenderer::meshes.size(); i++)
			{
				unsigned int tVBO;
				unsigned int tVAO;
				unsigned int tEBO;
				
				glGenBuffers(1, &tVBO);
				//glGenBuffers(1, &tEBO);
				glGenVertexArrays(1, &tVAO);

				glBindVertexArray(tVAO);

				glBindBuffer(GL_ARRAY_BUFFER, tVBO);
				glBufferData(GL_ARRAY_BUFFER, MeshRenderer::meshes[i].vertsize, &MeshRenderer::meshes[i].vertices[0], GL_STATIC_DRAW);
				/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, MeshRenderer::meshes[i].indsize, &MeshRenderer::meshes[i].indicies[0], GL_STATIC_DRAW);*/

				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(0);
				/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(1);*/
				glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
				glEnableVertexAttribArray(2);

				VAOs.push_back(tVAO);


				Screenshader.use();
				Screenshader.setInt("screenTexture", 0);
				glGenFramebuffers(1, &FBO);
				glBindFramebuffer(GL_FRAMEBUFFER, FBO);
				// create a color attachment texture
				glGenTextures(1, &textureColorbuffer);
				glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
				// create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
				unsigned int rbo;
				glGenRenderbuffers(1, &rbo);
				glBindRenderbuffer(GL_RENDERBUFFER, rbo);
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
				// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
				if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
					std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}
			state = 1;
			//texture.InitTexture("texture/test.jpg");
		}
		else if (state == 1)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glEnable(GL_DEPTH_TEST);
			for (int id : SceneManager::GetInstance()->currentScene->gameobject_IDs)
			{
				glm::vec3 position = Transform::position[id];
				glm::vec3 rotation = Transform::rotation[id];
				glm::vec3 scale = Transform::scale[id];
				if (MeshRenderer::renderables.find(id) != MeshRenderer::renderables.end())
				{

					texture.drawTexture();
					shader.use();
					glClear(GL_DEPTH_BUFFER_BIT);
					glm::mat4 model = glm::mat4(1.0f);
					glm::mat4 view = glm::mat4(1.0f);
					glm::mat4 projection = glm::mat4(1.0f);

					model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					view = glm::translate(view, position);
					projection = glm::perspective(glm::radians(45.0f), 500.0f / 300.0f, 0.1f, 100.0f);

					unsigned int modelLoc = glGetUniformLocation(shader.shader_ID, "model");
					unsigned int viewLoc = glGetUniformLocation(shader.shader_ID, "view");

					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

					shader.setMat4("projection", projection);


					glBindVertexArray(VAOs[MeshRenderer::renderables[id].first]);
					glDrawArrays(GL_TRIANGLES, 0, 36);

					//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
			
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			Screenshader.use();
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			glDrawArrays(GL_TRIANGLES, 0, 6);
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