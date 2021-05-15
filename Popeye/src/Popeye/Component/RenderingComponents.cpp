#include "RenderingComponents.h"
#include "../Manager/ResourceManager.h"

#include "stb_image.h"

namespace Popeye {
	extern ResourceManager* g_ResourceManager;

	/**************Shader**************/
	Shader::Shader(){}

	void Shader::Init(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexCode, fragmentCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			POPEYE_CORE_ERROR("Can't load shader.");
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader_ID = glCreateProgram();
		glAttachShader(shader_ID, vertex);
		glAttachShader(shader_ID, fragment);
		glLinkProgram(shader_ID);

		glGetProgramiv(shader_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader_ID, 512, NULL, infoLog);
			POPEYE_CORE_ERROR("program link fail");
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::Init(const GLchar* vertexPath, const GLchar* geometryPath, const GLchar* fragmentPath)
	{
		std::string vertexCode, geometryCode, fragmentCode;
		std::ifstream vShaderFile, gShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			gShaderFile.open(geometryPath);
			std::stringstream vShaderStream, gShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			gShaderStream << gShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			gShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			geometryCode = gShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			POPEYE_CORE_ERROR("Can't load shader.");
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* gShaderCode = geometryCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, geometry, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		shader_ID = glCreateProgram();
		glAttachShader(shader_ID, vertex);
		glAttachShader(shader_ID, fragment);
		glAttachShader(shader_ID, geometry);
		glLinkProgram(shader_ID);

		glGetProgramiv(shader_ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader_ID, 512, NULL, infoLog);
			POPEYE_CORE_ERROR("program link fail");
		}

		glDeleteShader(vertex);
		glDeleteShader(geometry);
		glDeleteShader(fragment);
	}

	void Shader::use()
	{
		glUseProgram(shader_ID);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), (int)value);
	}
	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), value);
	}
	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(shader_ID, name.c_str()), value);
	}
	void Shader::setVec3(const std::string& name, glm::vec3 value) const
	{
		glUniform3fv(glGetUniformLocation(shader_ID, name.c_str()), 1, &value[0]);
	}
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


	/*void Texture::DeleteTexture()
	{
		glDeleteTextures(1, &texture_ID);
		texture_ID = -1;
	}*/

	/**************Light**************/
	int Light::pointLightCounter		= 0;
	int Light::directionalLightCounter	= 0;
	int Light::spotLightCounter			= 0;

	Light::Light()
	{
		pointLightCounter++;

		SetValue();
	}
	
	void Light::ChangeLightType(LightType changeType)
	{
		if (type == LightType::POINT)					{ pointLightCounter--; }
		else if (type == LightType::DIRECTION)			{ directionalLightCounter--; }
		else if (type == LightType::SPOT)				{ spotLightCounter--; }

		if (changeType == LightType::POINT)				{ pointLightCounter++; }
		else if (changeType == LightType::DIRECTION)	{ directionalLightCounter++; }
		else if (changeType == LightType::SPOT)			{ spotLightCounter++; }

		type = changeType;
	}
	
	LightType Light::ShowLightType()
	{
		return type;
	}

	void Light::SetValue()
	{
		type = LightType::POINT;

		color = glm::vec3(1.0f);

		ambient = 0.8f;
		diffuse = 0.7f;
		specular = 0.1f;

		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;

		cutoff = 12.5f;
		outercutoff = 17.5f;
	}


	/**************MeshRenderer component**************/
	MeshRenderer::MeshRenderer() 
	{
		SetValue();
	}
	
	void MeshRenderer::SetValue()
	{
		meshID = 0;
		materialID = 0;
		isEmpty = true;
	}


	/*****************Camera component*****************/
	Camera::Camera()
	{
		SetValue();
	}

	void Camera::SetValue()
	{
		mod = Projection::PERSPECTIVE;
		fov = 45.0f;
		offsetX = 800.0f, offsetY = 600.0f;

		nearView = 0.1f, farView = 100.0f;

		width = 5.0f, height = 5.0f;
	}
}
