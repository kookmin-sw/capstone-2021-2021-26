#pragma once

namespace Popeye 
{
	struct BoundBox
	{
		float maxX, minX,
			maxY, minY,
			maxZ, minZ;
	};

	struct Mesh 
	{
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
		std::vector<Mesh> meshes;
	};

}