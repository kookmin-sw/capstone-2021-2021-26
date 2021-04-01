#pragma once

namespace Popeye 
{
	/*struct Vertex
	{
		float pos_norm_Tex[8] = { 0, };
	};*/

	struct Mesh 
	{
		unsigned int VAO, VBO, EBO;
		std::vector<float> vertices;
		std::vector<unsigned int> indices;

		Mesh(std::vector<float> _vertices, std::vector<unsigned int> _indices);
		void SetMesh();
	};

	struct Model
	{
		std::vector<Mesh> meshes;
	};

}