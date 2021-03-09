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
		void setVec3(const std::string& name, glm::vec3 value) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
	};

	struct Texture
	{
		unsigned int texture_ID;
		int width, height, nrChannel;
		
		Texture();
		void InitTexture(const char*);
		void drawTexture();
	};

	/*Light Component*/
	enum class LightType { POINT, DIRECTION, SPOT };
	struct Light
	{
		static int pointLightCounter;
		static int directionalLightCounter;
		static int spotLightCounter;

		glm::vec3 color;
		
		float ambient;
		float diffuse;
		float specular;

		//point & spot
		float constant;
		float linear;
		float quadratic;

		//spot
		float cutoff;
		float outercutoff;

		Light();
		void ChangeLightType(LightType);
		LightType ShowLightType();
	private:
		LightType type;
	};

	struct Material
	{
		std::string id;
		Texture texture;
		glm::vec3 color;
		glm::vec3 amb_diff_spec;
		float shininess;

		Material();
	};

	struct Mesh 
	{
		std::string id;
		float* vertices;
		int vertsize;
		unsigned int* indicies;
		int indsize;

		unsigned int VAO;
	};

	/*MeshRenderer*/
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


	/*Camera Component*/
	enum class Projection { PERSPECTIVE, ORTHOGRAPHIC };
	struct Camera
	{
		Projection mod = Projection::PERSPECTIVE;
		float fov = 45.0f;
		float offsetX = 800.0f, offsetY = 600.0f;

		/*Perspective view*/
		float nearView = 0.1f, farView = 100.0f;

		/*ORTHOGRAPHIC*/
		float width = 5.0f, height = 5.0f;
	};
}

