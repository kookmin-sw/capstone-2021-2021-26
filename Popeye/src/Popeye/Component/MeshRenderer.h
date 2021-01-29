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
	};

	struct Texture
	{
		unsigned int texture_ID;
		int width, height, nrChannel;
		void InitTexture(const char*);
		void drawTexture();
	};
}

