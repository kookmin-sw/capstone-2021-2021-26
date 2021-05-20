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

	//---------------------------------------
	//| Rendering Components
	//---------------------------------------
	//  All Component have own Constructor 
	//  and function name SetValue().
	// 

	/*Light Component*/
	enum class LightType { POINT, DIRECTION, SPOT };
	struct Light
	{
		static int pointLightCounter;
		static int directionalLightCounter;
		static int spotLightCounter;

		LightType type;
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
		void SetValue();

		void ChangeLightType(LightType);
		LightType ShowLightType();
	};

	/*MeshRenderer*/
	struct MeshRenderer
	{
		unsigned int meshID;
		unsigned int materialID;
		bool isEmpty;

		MeshRenderer();
		void SetValue();
	};


	/*Camera Component*/
	enum class Projection { PERSPECTIVE, ORTHOGRAPHIC };
	struct Camera
	{
		Projection mod;
		float fov;
		float offsetX, offsetY;

		/*Perspective view*/
		float nearView, farView;

		/*ORTHOGRAPHIC*/
		float width, height;

		Camera();
		void SetValue();
	};
}

