#include "Gizmo.h"

namespace Popeye {
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

				grid.indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
				grid.indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, grid.indices.size() * sizeof(glm::uvec4), glm::value_ptr(grid.indices[0]), GL_STATIC_DRAW);

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
			glm::uvec4(0, 1, 1, 2),
			glm::uvec4(2, 3, 3, 0),

			glm::uvec4(0, 4, 1, 5),
			glm::uvec4(2, 6, 3, 7),

			glm::uvec4(4, 5, 5, 6),
			glm::uvec4(6, 7, 7, 4)
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
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, wirecube.indices.size() * sizeof(glm::uvec4), glm::value_ptr(wirecube.indices[0]), GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		

		//length = indices.size() * 4;
	}


	void Gizmo::DrawGrid()
	{
		glBindVertexArray(grid.VAO);
		glLineWidth(1.5f);
		glDrawElements(GL_LINES, grid.indices.size() * 4, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}

	void Gizmo::DrawWireCube()
	{
		glBindVertexArray(wirecube.VAO);
		glLineWidth(1.0f);
		glDrawElements(GL_LINES, wirecube.indices.size() * 4, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}