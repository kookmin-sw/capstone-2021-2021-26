#include "pch.h"
#include "Display.h"
#include "GUI/GUIManager.h"
#include "Scene/GameObject.h"
#include "Component/Transform.h"
#include "Component/MeshRenderer.h"

namespace Popeye {
	Display::Display(){}
	Display::~Display(){}
	
	bool Display::init_Display()
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

	GLFWwindow* Display::get_window()
	{
		return this->Window;
	}

	void Display::run_Display()
	{

		static GUIManager* guimanager = new GUIManager();

		guimanager->onSet(this->get_window());

		float vertices[] = {
			0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		Popeye::mesh object;

		object.shader;
		object.texture.InitTexture("texture/test.jpg");

		object.init_buffer(vertices, sizeof(vertices), indices, sizeof(indices));
		//Popeye::Shader shader;
		//Popeye::Texture texture;


		//texture.InitTexture("texture/test.jpg");
		//Popeye::GameObject* gameObject = new GameObject(1);
		//gameObject->addComponent<Transform>();
		//gameObject->getComponent<Transform>().get_info(1);

		//Popeye::GameObject* gameObject2 = new GameObject(2);
		//gameObject2->addComponent<Transform>();

		
		while (!glfwWindowShouldClose(Window))
		{
			//guimanager->onRun();
			int display_w, display_h;
			glfwGetFramebufferSize(Window, &display_w, &display_h); //temp :: camera
			
			glClearColor(1.0f, 0.5f, 0.5f, 0.25f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			glViewport(0, 0, display_w / 2, display_h / 2);
			object.draw_mesh();

			glViewport(display_w / 2, 0, display_w/2, display_h/2); // temp :: scene view
			object.draw_mesh();

			//guimanager->onRunDraw();
			glfwSwapBuffers(Window);
			glfwPollEvents();
		}

		delete(guimanager);
	}

	void Display::close_Display()
	{
		glfwTerminate();
	}
}