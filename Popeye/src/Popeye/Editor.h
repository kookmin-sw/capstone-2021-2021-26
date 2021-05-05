#pragma once

namespace Popeye{
	struct BoundBox;

	enum class EditorMod
	{
		TRANSLATE,
		ROTATE,
		SCALE
	};

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

	class Editor 
	{
	private:
		unsigned int editorFBO;
		
		Gizmo gizmo;
		EditorMod mod;
	private:
		void RenderView();
		void ScreenToWorldPos(glm::vec2 mousePos, const glm::mat4& view, const glm::mat4& proj, glm::vec3& rayOrigin, glm::vec3& rayDir);
		bool RayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_end, BoundBox boundbox, const glm::mat4& model);

	public:
		glm::vec3 editorCamPos;
		glm::vec3 editorCamDir;
	public:
		Editor();
		~Editor();
		void Init();
		void OnRun();
	};
}