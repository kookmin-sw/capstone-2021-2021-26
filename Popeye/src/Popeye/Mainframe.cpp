#include "Mainframe.h"

#include "Manager/GUIManager.h"
#include "Manager/SceneManager.h"
#include "Manager/ComponentManager.h"
#include "Manager/ResourceManager.h"

#include "FileIO.h"
#include "Event/EventHandler.h"

#include "System/RenderingSystem.h"

#include "Scene/Scene.h"
#include "Scene/GameObject.h"

#include "Component/RenderingComponents.h"

namespace Popeye {
	FileIO* g_fileIO;
	
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
		EventHandler* eventHandler = new EventHandler();
		eventHandler->SetEventCallbacks(window);

		GUIManager* guiManager = new GUIManager();
		guiManager->OnSet(window);

		g_fileIO = new FileIO();
		g_fileIO->Init();

		g_ResourceManager = new ResourceManager();

		SceneManager::GetInstance()->currentScene = new Scene();

		RenderingSystem *renderingSystem = new RenderingSystem();
		renderingSystem->SystemInit();
		

		ComponentManager::GetInstance()->InitComponents();



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
		scene->gameObjects[1]->transform.scale	  =	{ 0.5f, 0.5f , 0.5f };
		scene->gameObjects[1]->transform.rotation =	{ -90.0f, -30.0f , 0.0f };
		scene->gameObjects[1]->transform.position = { 5.0f, 10.0f, 5.0f };

		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			

			renderingSystem->SystemRunning();
			
			eventHandler->HandleEvent();

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

		delete(g_fileIO);

		delete(eventHandler);
		delete(renderingSystem);

	}

	void Mainframe::Close()
	{
		glfwTerminate();
	}
}