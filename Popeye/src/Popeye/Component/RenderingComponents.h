#pragma once
#define GET_NAME(name) #name
namespace Popeye {
	struct Shader
	{
		unsigned int shader_ID;
		Shader(const GLchar* vertexPath = "shader/vertexShader.glsl", const GLchar* fragmentPath = "shader/fragmentShader.glsl");
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};

	struct Texture
	{
		unsigned int texture_ID;
		Texture();
		int width, height, nrChannel;
		void InitTexture(const char*);
		void drawTexture();
	};

	struct Material
	{
		std::string id;
		Texture texture;
		glm::vec3 albedo;
		float metalic;
		float smoothness;
	};

	struct Mesh 
	{
		std::string id;
		float* vertices;
		int vertsize;
		unsigned int* indicies;
		int indsize;
	};

	struct MeshRenderer
	{
		static std::vector<Mesh> meshes;
		static std::vector<Material> materials;

		int meshIndex = 0;
		int materialIndex = 0;

		MeshRenderer();
		void SetMesh(Mesh&);
		void SetMaterial(Material&);
	};


	enum class Projection { PERSPECTIVE, ORTHOGRAPHIC };
	struct Camera
	{
		void SetMainCamera();

		Projection mod = Projection::ORTHOGRAPHIC;
		float fov = 45.0f;
		float nearView = 0.1f, farView = 100.0f;
		float offsetX = 800.0f, offsetY = 600.0f;
		float size = 20.0f;
	};

}

