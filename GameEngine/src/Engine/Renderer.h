#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer
{
	GLFWwindow* Window;
public:
	Renderer();
	virtual ~Renderer();

	GLFWwindow* get_window();
	
	void set_render();
	bool is_window_should_close();


};

