#include "pch.h"
#include "Display.h"
#include "GUI/GUIManager.h"
#include "System/RenderingSystem.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Component/Transform.h"
#include "Component/RenderingComponents.h"

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
		Scene* scene = new Scene();

		static GUIManager* guimanager = new GUIManager();

		guimanager->onSet(this->get_window());

		float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};
		RenderingSystem renderer;
		Popeye::Mesh object;
		object.id = GET_NAME(object);
		object.vertsize = sizeof(vertices);
		object.vertices = vertices;
		object.indicies = indices;
		object.indsize = sizeof(indices);

		//Popeye::mesh m;
		//m.shader;
		//m.texture.InitTexture("texture/test.jpg");
		//m.init_buffer(vertices, sizeof(vertices), indices, sizeof(indices));
		
		Popeye::GameObject* gameObject = new GameObject();
		gameObject->AddComponent<Transform>();
		gameObject->AddComponent<MeshRenderer>();
		gameObject->GetComponent<MeshRenderer>().SetMesh(gameObject->GetID(), object);
		gameObject->GetComponent<Transform>().Set_pos(gameObject->GetID(), { -1.0f, 0.0f, -3.0f });

		Popeye::GameObject* gameObject2 = new GameObject();
		gameObject2->AddComponent<Transform>();
		gameObject2->AddComponent<MeshRenderer>();
		gameObject2->GetComponent<MeshRenderer>().SetMesh(gameObject2->GetID(), object);
		gameObject2->GetComponent<Transform>().Set_pos(gameObject2->GetID(), { 1.0f, 0.0f, -3.0f });

		Popeye::GameObject* gameObject3 = new GameObject();
		gameObject3->AddComponent<Transform>();
		gameObject3->AddComponent<MeshRenderer>();
		gameObject3->GetComponent<MeshRenderer>().SetMesh(gameObject3->GetID(), object);
		gameObject3->GetComponent<Transform>().Set_pos(gameObject3->GetID(), { 1.0f, 1.0f, -3.0f });
		gameObject3->GetComponent<Transform>().Set_scale(gameObject3->GetID(), { 1.0f, 1.0f, 2.0f });

		while (!glfwWindowShouldClose(Window))
		{
			//guimanager->onRun();
			int display_w, display_h;
			glfwGetFramebufferSize(Window, &display_w, &display_h);
			glViewport(0, 0, display_w, display_h);
			glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			
			//m.draw_mesh();

			renderer.SystemRunning();
			//glViewport(display_w / 2, 0, display_w/2, display_h/2); // temp :: scene view
			//object.draw_mesh();

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