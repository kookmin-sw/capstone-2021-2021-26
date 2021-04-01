#include "Mainframe.h"

#include "Manager/GUIManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ComponentManager.h"
#include "Manager/ResourceManager.h"

#include "System/FileSystem.h"
#include "System/RenderingSystem.h"
#include "System/EventSystem.h"

#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Component/RenderingComponents.h"

namespace Popeye {
	FileSystem* g_fileSystem;
	ResourceManager* g_ResourceManager;


	Mainframe::Mainframe(){}
	Mainframe::~Mainframe(){}

	bool Mainframe::Init()
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

	void Mainframe::Run()
	{
		g_fileSystem = new FileSystem();

		g_ResourceManager = new ResourceManager();

		GUIManager* guiManager = new GUIManager();

		SceneManager::GetInstance()->currentScene = new Scene();

		RenderingSystem *renderingSystem = new RenderingSystem();
		
		EventSystem *eventSystem = new EventSystem();

		ComponentManager::GetInstance()->InitComponents();

		g_fileSystem->Init();

		renderingSystem->SystemInit();
		eventSystem->SetEventCallbacks(window);
		guiManager->OnSet(window);


		Scene* scene = SceneManager::GetInstance()->currentScene;
		scene->SetName("example");

		Popeye::Material material_0;
		material_0.id = GET_NAME(material_0);
		material_0.color = glm::vec3(1.0f, 0.5f, 0.5f);
		
		Popeye::Material material_1;
		material_1.id = GET_NAME(material_1);
		material_1.color = glm::vec3(1.0f, 1.0f, 1.0f);

		scene->CreateGameObject();
		scene->CreateGameObject();
		

		scene->gameObjects[0]->SetName("Camera");
		scene->gameObjects[0]->AddComponent<Camera>();
		scene->mainCameraID = scene->gameObjects[0]->GetID();
		scene->gameObjects[0]->transform.position = { 8.0f, 8.0f, 8.0f };
		scene->gameObjects[0]->transform.rotation = { -30.0f, 45.0f, 0.0f };

		scene->gameObjects[1]->SetName("Directional Light");
		scene->gameObjects[1]->AddComponent<Light>();
		scene->gameObjects[1]->GetComponent<Light>().ChangeLightType(LightType::DIRECTION);
		scene->gameObjects[1]->GetComponent<MeshRenderer>().SetMaterial(material_0);
		scene->gameObjects[1]->transform.scale	  =	{ 0.5f, 0.5f , 0.5f };
		scene->gameObjects[1]->transform.rotation =	{ -90.0f, -30.0f , 0.0f };
		scene->gameObjects[1]->transform.position = { 5.0f, 10.0f, 5.0f };

		POPEYE_CORE_INFO("deededede");
		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			
			renderingSystem->SystemRunning();
			
			eventSystem->SystemRunning();

			guiManager->OnRun();

			glfwSwapBuffers(window);
		}
		
		//TODO:: fix destructor more fancy way later..

		//free managers
		SceneManager::GetInstance()->UnloadScene();
		SceneManager::GetInstance()->DestroyInstance();

		ComponentManager::GetInstance()->DestroyInstance();

		guiManager->OnClosed();
		delete(guiManager);

		delete(g_fileSystem);

		delete(eventSystem);
		delete(renderingSystem);

	}

	void Mainframe::Close()
	{
		glfwTerminate();
	}
}