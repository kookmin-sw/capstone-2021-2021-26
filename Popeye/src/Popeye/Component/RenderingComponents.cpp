#include "RenderingComponents.h"
#include "../Manager/ResourceManager.h"

#include "stb_image.h"

namespace Popeye {
	extern ResourceManager* g_ResourceManager;

	/**************Shader**************/
	Shader::Shader(){}

	void Shader::Init(const GLchar* vertexPath, const GLchar* fragmentPath)
	{
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

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
		type		= LightType::POINT;
		pointLightCounter++;

		color		= glm::vec3(1.0f);
		
		ambient		= 0.8f;
		diffuse		= 0.7f;
		specular	= 0.1f;

		constant	= 1.0f;
		linear		= 0.09f;
		quadratic	= 0.032f;

		cutoff		= 12.5f;
		outercutoff = 17.5f;
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


	/**************MeshRenderer component**************/
	MeshRenderer::MeshRenderer() 
	{
		meshID		= 0;
		materialID	= 0;
		isEmpty		= true;
	}
}
