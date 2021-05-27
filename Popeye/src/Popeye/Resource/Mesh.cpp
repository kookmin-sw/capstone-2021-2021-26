 #include "Mesh.h"

namespace Popeye
{
	Mesh::Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices, BoundBox _boundbox)
	{
		vertices = _vertices;
		indices = _indices;
		boundbox = _boundbox;
		SetMesh();
	}

	void Mesh::SetMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
		
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

		glBindVertexArray(0);
	}

	void Mesh::DrawMesh()
	{
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
	
	void Model::DrawModel()
	{
		int size = meshes.size();
		for (int i = 0; i < size; i++)
		{
			meshes[i].DrawMesh();
		}
	}

	void Model::SetBoundBox()
	{
		BoundBox _boundbox;
		_boundbox.maxPos = glm::vec3(0.0f);
		_boundbox.minPos = glm::vec3(0.0f);

		int size = meshes.size();
		for (int i = 0; i < size; i++)
		{
			BoundBox box = meshes[i].boundbox;
			if (_boundbox.maxPos.x < box.maxPos.x)
				_boundbox.maxPos.x = box.maxPos.x;
			if (_boundbox.minPos.x > box.minPos.x)
				_boundbox.minPos.x = box.minPos.x;

			if (_boundbox.maxPos.y < box.maxPos.y)
				_boundbox.maxPos.y = box.maxPos.y;
			if (_boundbox.minPos.y > box.minPos.y)
				_boundbox.minPos.y = box.minPos.y;

			if (_boundbox.maxPos.z < box.maxPos.z)
				_boundbox.maxPos.z = box.maxPos.z;
			if (_boundbox.minPos.z > box.minPos.z)
				_boundbox.minPos.z = box.minPos.z;
		}

		boundbox = _boundbox;
	}
}