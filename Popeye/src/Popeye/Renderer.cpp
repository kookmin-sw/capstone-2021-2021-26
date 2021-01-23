#include "pch.h"
#include "Renderer.h"
#include "GUI/GUIManager.h"
#include "mesh.h"

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


namespace Popeye {
	Renderer::Renderer(){}
	Renderer::~Renderer(){}
	
	bool Renderer::init_renderer()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->Window = glfwCreateWindow(1200, 600, "Popeye Engine", NULL, NULL);
		if (!this->Window)
		{
			return false;
		}
		glfwMakeContextCurrent(this->Window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}
		return true;
	}

	GLFWwindow* Renderer::get_window()
	{
		return this->Window;
	}

	void Renderer::run_renderer()
	{
		//shader
		unsigned int vertexShader, fragmentShader, shaderProgram;
		
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		//shader end.

		static GUIManager* guimanager = new GUIManager();

		guimanager->onSet(this->get_window());

		float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
		};

		Popeye::mesh* object = new mesh();
		object->init_buffer(vertices, sizeof(vertices));

		while (!glfwWindowShouldClose(Window))
		{
			guimanager->onRun();
			int display_w, display_h;
			glfwGetFramebufferSize(Window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(1.0f, 0.5f, 0.5f, 0.25f);
			glClear(GL_COLOR_BUFFER_BIT);

			glUseProgram(shaderProgram);
			object->draw_mesh();

			guimanager->onRunDraw();
			glfwSwapBuffers(Window);
			glfwPollEvents();
		}

		delete(guimanager);
	}

	void Renderer::close_renderer()
	{
		glfwTerminate();
	}
}