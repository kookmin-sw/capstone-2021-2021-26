#include "pch.h"
#include "MeshRenderer.h"
namespace Popeye {
	Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
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
		}

	}
}
