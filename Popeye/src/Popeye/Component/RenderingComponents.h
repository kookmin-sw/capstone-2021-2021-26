#pragma once

#define GET_NAME(name) #name
namespace Popeye {

	struct Shader
	{
		unsigned int shader_ID;
		
		Shader();
		void Init(const GLchar* vertexPath, const GLchar* fragmentPath);
		void Init(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath);
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setVec3(const std::string& name, glm::vec3 value) const;
		void setMat4(const std::string& name, const glm::mat4& mat) const;
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

	/*MeshRenderer*/
	struct MeshRenderer
	{
		unsigned int meshID;
		unsigned int materialID;
		bool isEmpty;

		MeshRenderer();
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

