#include "pch.h"
#include "RenderingComponents.h"
#include "stb_image.h"

namespace Popeye {

	/**************Shader define**************/
	Shader::Shader(const GLchar* vertexPath , const GLchar* fragmentPath)
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
		catch(std::ifstream::failure e)
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
		glUniform1i(glGetUniformLocation(shader_ID, name.c_str()), value);
	}
	void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(shader_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}


	/**************Texture define**************/
	Texture::Texture() {};

	void Texture::InitTexture(const char* imgPath)
	{
		glGenTextures(1, &texture_ID);
		glBindTexture(GL_TEXTURE_2D, texture_ID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* data = stbi_load(imgPath, &width, &height, &nrChannel, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			POPEYE_CORE_ERROR("Failed to load texture");
		}

		stbi_image_free(data);
	}

	void Texture::drawTexture()
	{
		glActiveTexture(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, this->texture_ID);
	}


	/**************MeshRenderer component**************/
	//MeshRenderer::MeshRenderer() {}

	std::vector<Mesh>		MeshRenderer::meshes;
	std::vector<Material>	MeshRenderer::materials;

	MeshRenderer::MeshRenderer() {}

	void MeshRenderer::SetMesh(Mesh& mesh)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			if (meshes[i].id == mesh.id)
			{
				meshIndex = i;
				return;
			}
		}


		unsigned int tVBO;
		glGenBuffers(1, &tVBO);
		glBindBuffer(GL_ARRAY_BUFFER, tVBO);

		glGenVertexArrays(1, &mesh.VAO);
		glBindVertexArray(mesh.VAO);

		/*unsigned int tEBO;
		glGenBuffers(1, &tEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tEBO);*/

		glBufferData(GL_ARRAY_BUFFER, mesh.vertsize, &mesh.vertices[0], GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indsize, &mesh.indicies[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		/*glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);*/
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);

		meshes.push_back(mesh);
		meshIndex = meshes.size() - 1;
	}
}
