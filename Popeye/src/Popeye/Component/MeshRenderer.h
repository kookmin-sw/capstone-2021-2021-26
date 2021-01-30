#pragma once
namespace Popeye {

	struct MeshRenderer
	{
		MeshRenderer();
	};
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

	struct Material
	{
		Shader shader;
		Texture texture;
		glm::vec3 albedo;
		float metalic;
		float smoothness;
	};

	struct Texture
	{
		unsigned int texture_ID;
		Texture();
		int width, height, nrChannel;
		void InitTexture(const char*);
		void drawTexture();
	};

	struct mesh
	{
		Shader shader;
		Texture texture;
		mesh();
		~mesh();
		void init_buffer(float*, int, unsigned int*, int);
		void draw_mesh();
	private:
		unsigned int VBO;
		unsigned int VAO;
		unsigned int EBO;
	};

	struct Renderer
	{
		static std::vector<Material> materials;
		static std::vector<mesh> meshes;
		static std::unordered_map<int, std::pair<int, int>> renderables;

		static void componentAdded(int);
		static void setMesh(int, mesh);
		static mesh& getMesh(int);
		static void setMaterial(int);
		static Material& getMaterial(int);
		Renderer();
	};
}

