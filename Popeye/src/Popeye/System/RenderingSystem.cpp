#include "pch.h"
#include "RenderingSystem.h"

#include "../Scene/Scene.h"
#include "../Component/Transform.h"
#include "../Component/RenderingComponents.h"
#include "../Component/Camera.h"

namespace Popeye {

	// render system should run while not on play...
	void RenderingSystem::SystemRunning()
	{
		static int state = 0; //temp state;
		static Shader shader;
		static Texture texture;
		if (state == 0)
		{
			Camera camera;
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
			}
			state = 1;
			texture.InitTexture("texture/test.jpg");
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
				Cam camera;
				glm::vec3 camPos;
				if (Camera::cameras.find(id) != Camera::cameras.end())
				{
					camera = Camera::cameras[id];
					camPos = position;
					std::cout << camPos.x << " " << camPos.y << " " << camPos.z << std::endl;

				}

				if (MeshRenderer::renderables.find(id) != MeshRenderer::renderables.end())
				{

					texture.drawTexture();
					shader.use();
					glClear(GL_DEPTH_BUFFER_BIT);
					glm::mat4 model = glm::mat4(1.0f);
					glm::mat4 view = glm::mat4(1.0f);
					glm::mat4 projection = glm::mat4(1.0f);

					model = glm::translate(model, position);
					
					model = glm::scale(model, scale);
					
					model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
					model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
					model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

					unsigned int modelLoc = glGetUniformLocation(shader.shader_ID, "model");
					unsigned int viewLoc = glGetUniformLocation(shader.shader_ID, "view");

					glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
					glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

					//add Camera
					view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
					if (camera.mod == CamMod::PERSPECTIVE)
					{
					}
					else if (camera.mod == CamMod::ORTHO)
					{
						projection = glm::perspective(camera.fov, camera.offsetX / camera.offsetY, camera.nearView, camera.farView);
						//projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
					}
					shader.setMat4("projection", projection);


					glBindVertexArray(VAOs[MeshRenderer::renderables[id].first]);
					glDrawArrays(GL_TRIANGLES, 0, 36);

					//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}

			}
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