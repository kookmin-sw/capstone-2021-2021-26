#pragma once
#define GET_NAME(name) #name
namespace Popeye {
	struct Shader
	{
		unsigned int shader_ID;
		Shader(const GLchar* vertexPath = "shader/vertexShader.GLSL", const GLchar* fragmentPath = "shader/fragmentShader.GLSL");
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
		MeshRenderer();

		static std::vector<Mesh> meshes;
		static std::vector<Material> materials;
		static std::unordered_map<int, std::pair< int, int>> renderables;

		static void ComponentAdded(int);
		static void SetMesh(int, Mesh&);
		//static mesh& getMesh(int);
		static void SetMaterial(int, Material&);
		//static Material& getMaterial(int);
	};
}

