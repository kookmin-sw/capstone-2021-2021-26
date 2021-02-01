#include "pch.h"
#include "RenderingSystem.h"

#include "../Component/Transform.h"
#include "../Component/RenderingComponents.h"

namespace Popeye {
	
	// render system should run while not on play...
	void RenderingSystem::SystemRunning()
	{
		static int state = 0; //temp state;
		static Shader shader;
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
			}
			state = 1;
			texture.InitTexture("texture/test.jpg");
		}
		else if (state == 1)
		{
			for (auto pos : Transform::position)
			{
				int id = pos.first;
				if (MeshRenderer::renderables.find(id) != MeshRenderer::renderables.end())
				{
					texture.drawTexture();
					shader.use();
					glClear(GL_DEPTH_BUFFER_BIT);
					glm::mat4 model = glm::mat4(1.0f);
					glm::mat4 view = glm::mat4(1.0f);
					glm::mat4 projection = glm::mat4(1.0f);

					model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
					view = glm::translate(view, pos.second);
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