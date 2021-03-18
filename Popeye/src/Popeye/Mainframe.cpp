#include "Mainframe.h"

#include "Manager/FileManager.h"
#include "Manager/GUIManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ComponentManager.h"

#include "System/RenderingSystem.h"
#include "System/EventSystem.h"

#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Component/RenderingComponents.h"
namespace Popeye {

	FileManager* fileManager;

	Mainframe::Mainframe(){}
	Mainframe::~Mainframe(){}

	bool Mainframe::Init()
	{

		/****glfw*****/
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
		/*************/
		return true;
	}

	void Mainframe::Run()
	{
		fileManager = new FileManager();

		ComponentManager::GetInstance()->InitComponents();

		EventSystem* eventSystem = new EventSystem();
		eventSystem->SetEventCallbacks(window);

		GUIManager* guimanager = new GUIManager();
		guimanager->OnSet(window);

		Scene* scene = new Scene();
		scene->SetName("example");

		SceneManager::GetInstance()->currentScene = scene;

		float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f
		};

		float vertices2[] = {
		-0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.5f, 0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 0.0f, 0.5f, 0.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f,  0.0f, 0.0f,
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		RenderingSystem* renderingSystem = new RenderingSystem();

		Popeye::Mesh cube;
		cube.id = GET_NAME(object_0);
		cube.vertsize = sizeof(vertices);
		cube.vertices = vertices;
		cube.indicies = indices;
		cube.indsize = sizeof(indices);

		Popeye::Material material_0;
		material_0.id = GET_NAME(material_0);
		material_0.color = glm::vec3(1.0f, 0.5f, 0.5f);
		
		Popeye::Material material_1;
		material_1.id = GET_NAME(material_1);
		material_1.color = glm::vec3(1.0f, 1.0f, 1.0f);

		scene->CreateGameObject();
		scene->CreateGameObject();
		scene->CreateGameObject();
		scene->CreateGameObject();
		scene->CreateGameObject();
		scene->CreateGameObject();

		scene->gameObjects[0]->SetName("gameObject1");
		scene->gameObjects[0]->AddComponent<MeshRenderer>();
		scene->gameObjects[0]->GetComponent<MeshRenderer>().SetMesh(cube);
		scene->gameObjects[0]->GetComponent<MeshRenderer>().SetMaterial(material_1);
		scene->gameObjects[0]->transform.position = { 0.0f, 0.0f, 0.0f };
		scene->gameObjects[0]->transform.scale = { 1.0f, 2.0f, 1.0f };

		scene->gameObjects[1]->SetName("gameObject2");
		scene->gameObjects[1]->AddComponent<MeshRenderer>();
		scene->gameObjects[1]->GetComponent<MeshRenderer>().SetMesh(cube);
		scene->gameObjects[1]->GetComponent<MeshRenderer>().SetMaterial(material_0);
		scene->gameObjects[1]->transform.position = { 2.0f, 3.0f, 3.0f };

		scene->gameObjects[2]->SetName("Camera");
		scene->gameObjects[2]->AddComponent<Camera>();
		scene->mainCameraID = scene->gameObjects[2]->GetID();
		scene->gameObjects[2]->transform.position = { 8.0f, 8.0f, 8.0f };
		scene->gameObjects[2]->transform.rotation = { -30.0f, 45.0f, 0.0f };

		scene->gameObjects[3]->SetName("Light");
		scene->gameObjects[3]->AddComponent<Light>();
		scene->gameObjects[3]->GetComponent<Light>().ChangeLightType(LightType::POINT);
		scene->gameObjects[3]->AddComponent<MeshRenderer>();
		scene->gameObjects[3]->GetComponent<MeshRenderer>().SetMesh(cube);
		scene->gameObjects[3]->transform.scale	  =	{ 0.5f, 0.5f , 0.5f };
		scene->gameObjects[3]->transform.rotation =	{ -90.0f, -30.0f , 0.0f };
		scene->gameObjects[3]->transform.position = { 5.0f, 10.0f, 5.0f };

		scene->gameObjects[4]->SetName("gameObject3");
		scene->gameObjects[4]->AddComponent<MeshRenderer>();
		scene->gameObjects[4]->GetComponent<MeshRenderer>().SetMesh(cube);
		scene->gameObjects[4]->GetComponent<MeshRenderer>().SetMaterial(material_0);
		scene->gameObjects[4]->transform.position = { 2.0f, 2.0f, 2.0f };

		scene->gameObjects[5]->SetName("gameObject4");
		scene->gameObjects[5]->AddComponent<MeshRenderer>();
		scene->gameObjects[5]->GetComponent<MeshRenderer>().SetMesh(cube);
		scene->gameObjects[5]->GetComponent<MeshRenderer>().SetMaterial(material_0);
		scene->gameObjects[5]->transform.position = { 3.0f, 3.0f, 3.0f };

		POPEYE_CORE_INFO("sds");
		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			renderingSystem->SystemRunning();
			
			eventSystem->SystemRunning();

			guimanager->OnRun();

			glfwSwapBuffers(window);
		}

		guimanager->OnClosed();
	}

	void Mainframe::Close()
	{
		glfwTerminate();
	}
}