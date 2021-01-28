#pragma once
namespace Popeye {
	struct MeshRenderer
	{
		MeshRenderer();
	};

	struct Shader
	{
		unsigned int shader_ID;
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		void use();
		void setBool(const std::string &name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
	};
}

