#include "pch.h"
#include "Mainframe.h"
#include "GUI/GUIManager.h"
#include "System/RenderingSystem.h"
#include "System/EventSystem.h"
#include "Scene/Scene.h"
#include "Scene/GameObject.h"
#include "Component/Transform.h"
#include "Component/RenderingComponents.h"
#include "Component/Camera.h"

namespace Popeye {
	Mainframe::Mainframe(){}
	Mainframe::~Mainframe(){}

	bool Mainframe::init_Display()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		this->window = glfwCreateWindow(1200, 600, "Popeye Engine", NULL, NULL);
		if (!this->window)
		{
			return false;
		}
		glfwMakeContextCurrent(this->window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			return false;
		}
		return true;
	}

	GLFWwindow* Mainframe::get_window()
	{
		return this->window;
	}

	void Mainframe::run_Display()
	{
		Scene* scene = new Scene();

		static GUIManager* guimanager = new GUIManager();

		EventSystem* eventsystem = new EventSystem();
		eventsystem->SetEventCallbacks(window);
		guimanager->OnSet(window);

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
		RenderingSystem* renderer = new RenderingSystem();
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
		gameObject->GetComponent<Transform>().Set_pos(gameObject->GetID(), { 0.0f, 0.0f, 0.0f });
		gameObject->GetComponent<Transform>().Set_scale(gameObject->GetID(), { 1.0f, 10.0f, 1.0f });

		Popeye::GameObject* gameObject2 = new GameObject();
		gameObject2->AddComponent<Transform>();
		gameObject2->AddComponent<MeshRenderer>();
		gameObject2->GetComponent<MeshRenderer>().SetMesh(gameObject2->GetID(), object);
		gameObject2->GetComponent<Transform>().Set_pos(gameObject2->GetID(), { 2.0f, 3.0f, 3.0f });
		gameObject2->GetComponent<Transform>().Set_scale(gameObject2->GetID(), { 1.0f, 1.0f, 1.0f });

		Popeye::GameObject* gameObject3 = new GameObject();
		gameObject3->AddComponent<Transform>();
		gameObject3->AddComponent<MeshRenderer>();
		gameObject3->GetComponent<MeshRenderer>().SetMesh(gameObject3->GetID(), object);
		gameObject3->GetComponent<Transform>().Set_pos(gameObject3->GetID(), { 2.0f, 0.0f, -1.0f });
		gameObject3->GetComponent<Transform>().Set_scale(gameObject3->GetID(), { 1.0f, 2.0f, 1.0f });

		Popeye::GameObject* gameObject6 = new GameObject();
		gameObject6->AddComponent<Transform>();
		gameObject6->AddComponent<MeshRenderer>();
		gameObject6->GetComponent<MeshRenderer>().SetMesh(gameObject6->GetID(), object);
		gameObject6->GetComponent<Transform>().Set_pos(gameObject6->GetID(), { 1.0f, 1.0f, 1.0f });
		gameObject6->GetComponent<Transform>().Set_scale(gameObject6->GetID(), { 1.0f, 1.0f, 1.0f });
		gameObject6->GetComponent<Transform>().Set_rotation(gameObject6->GetID(), { 0.0f, 0.0f, 150.0f });

		Popeye::GameObject* gameObject4 = new GameObject();
		gameObject4->AddComponent<Transform>();
		gameObject4->GetComponent<Transform>().Set_pos(gameObject4->GetID(), { 5.0f, 5.0f, 5.0f });
		gameObject4->AddComponent<Camera>();

		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			renderer->SystemRunning();
			
			eventsystem->SystemRunning();

			guimanager->OnRun();

			glfwSwapBuffers(window);
		}

		guimanager->OnClosed();
		delete(guimanager);
		delete(renderer);
	}

	void Mainframe::close_Display()
	{
		glfwTerminate();
	}
}