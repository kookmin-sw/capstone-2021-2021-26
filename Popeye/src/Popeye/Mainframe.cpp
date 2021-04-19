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

<<<<<<< HEAD
	void Mainframe::run_Display()
	{
		Scene* scene = new Scene();
		SceneManager::GetInstance()->currentScene = scene;
=======
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
>>>>>>> d3fea4472f4c492ecb6f3e9bd38da674be1267f9
		scene->SetName("example");

		Popeye::Material material_0;
		material_0.id = GET_NAME(material_0);
		material_0.color = glm::vec3(1.0f, 0.5f, 0.5f);
		
		Popeye::Material material_1;
		material_1.id = GET_NAME(material_1);
		material_1.color = glm::vec3(1.0f, 1.0f, 1.0f);

		scene->CreateGameObject();
		scene->CreateGameObject();
<<<<<<< HEAD
		scene->CreateGameObject();
		scene->CreateGameObject();
		scene->CreateGameObject();

		scene->gameObjects[0]->AddComponent<MeshRenderer>();
		scene->gameObjects[0]->GetComponent<MeshRenderer>().SetMesh(scene->gameObjects[0]->GetID(), object);
		scene->gameObjects[0]->transform.SetPosition({ 0.0f, 0.0f, 0.0f });
		scene->gameObjects[0]->transform.SetScale({ 1.0f, 2.0f, 1.0f });
		scene->gameObjects[0]->SetName("gameObject1");

		scene->gameObjects[1]->AddComponent<MeshRenderer>();
		scene->gameObjects[1]->GetComponent<MeshRenderer>().SetMesh(scene->gameObjects[1]->GetID(), object);
		scene->gameObjects[1]->transform.SetPosition({ 2.0f, 3.0f, 3.0f });
		scene->gameObjects[1]->SetName("gameObject2");

		scene->gameObjects[2]->AddComponent<MeshRenderer>();
		scene->gameObjects[2]->GetComponent<MeshRenderer>().SetMesh(scene->gameObjects[2]->GetID(), object);
		scene->gameObjects[2]->transform.SetPosition({ 2.0f, 0.0f, -1.0f });
		scene->gameObjects[2]->transform.SetScale({ 1.0f, 2.0f, 1.0f });
		scene->gameObjects[2]->SetName("gameObject3");

		scene->gameObjects[3]->transform.SetPosition( {5.0f, 5.0f, 5.0f} );
		scene->gameObjects[3]->AddComponent<Camera>();
		scene->gameObjects[3]->SetName("Camera");

		scene->gameObjects[4]->AddComponent<MeshRenderer>();
		scene->gameObjects[4]->GetComponent<MeshRenderer>().SetMesh(scene->gameObjects[4]->GetID(), object);
		scene->gameObjects[4]->transform.SetPosition({ 1.0f, 4.0f, 5.0f });
		scene->gameObjects[4]->transform.SetScale({ 6.0f, 2.0f, 6.0f });
		scene->gameObjects[4]->SetName("gameObject4");
=======
>>>>>>> d3fea4472f4c492ecb6f3e9bd38da674be1267f9
		

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