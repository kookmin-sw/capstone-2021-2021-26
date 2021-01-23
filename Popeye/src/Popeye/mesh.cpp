#include "pch.h"
#include "mesh.h"

namespace Popeye 
{
	mesh::mesh() {}
	mesh::~mesh() {}
	void mesh::init_buffer(float* vertices, int size)
	{
		glGenBuffers(1, &VBO);
		glGenVertexArrays(1, &VAO);
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, size, &vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void mesh::draw_mesh()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
	}
}
