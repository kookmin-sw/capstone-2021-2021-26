#pragma once

namespace Popeye 
{
	struct Vertex
	{
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 texCoord;
	};

	struct Mesh 
	{
		std::string id;
		std::vector<Vertex> vert;
		Mesh();
		void MeshInit();
	};

}