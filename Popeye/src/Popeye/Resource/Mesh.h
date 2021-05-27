#pragma once

namespace Popeye 
{
	struct BoundBox
	{
		glm::vec3 maxPos;
		glm::vec3 minPos;
	};

	struct Mesh 
	{
		float* id;
		std::string name;
		BoundBox boundbox;
		unsigned int VAO, VBO, EBO;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices, BoundBox _boundbox);
		void SetMesh();
		void DrawMesh();
	};

	struct Model
	{
		std::string name;
		BoundBox boundbox;
		std::vector<Mesh> meshes;
		void SetBoundBox();
		void DrawModel();
	};

}