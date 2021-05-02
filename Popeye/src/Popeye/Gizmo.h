#pragma once

namespace Popeye{

	struct GizmoMesh
	{
		unsigned int VAO;
		std::vector<glm::vec3> vertices;
		std::vector<glm::uvec2> indices;
	};

	class Gizmo 
	{
		GizmoMesh grid, wirecube, axis;
	public:
		Gizmo();
		~Gizmo();
		void Init();
		void DrawAxis();
		void DrawGrid();
		void DrawWireCube();
	};
}