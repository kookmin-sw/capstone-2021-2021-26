#include "Mainframe.h"

#include "Manager/GUIManager.h"
#include "Manager/ComponentManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"

#include "Editor.h"

#include "FileIO.h"
#include "Event/EventHandler.h"

#include "System/RenderingSystem.h"
#include "System/ScriptingSystem.h"
#include "System/PhysicsSystem.h"
#include "System/UISystem.h"


namespace Popeye {
	
	bool isPlay = false;

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
		g_fileIO = new FileIO();
		g_fileIO->Init();

		EventHandler *eventHandler = new EventHandler();
		eventHandler->SetEventCallbacks(window);

		// ------------------------------
		// On Engine 
		// ------------------------------
		Editor *editor = new Editor();
		editor->Init();
		{
			eventHandler->editor = editor;
		}

		GUIManager *guiManager = new GUIManager();
		guiManager->OnSet(window);
		// ------------------------------

		g_ResourceManager = new ResourceManager();

		ComponentManager::GetInstance()->InitComponents();

		SceneManager::GetInstance()->CreateScene();

		g_fileIO->InitProject(fs::current_path() / "popeye.info");
		g_ResourceManager->SetResources();

		// ------------------------------
		// Systems for game
		// ------------------------------
		RenderingSystem* renderingSystem = new RenderingSystem();
		renderingSystem->SystemInit();

		ScriptingSystem* scriptingSystem = new ScriptingSystem();
		scriptingSystem->SystemInit();

		PhysicsSystem* physicsSystem = new PhysicsSystem();
		physicsSystem->SystemInit(&isPlay);

		UISystem* uiSystem = new UISystem();
		uiSystem->SystemInit(&isPlay);

		// ------------------------------

		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			
			// ------------------------------
			//  Run rendering system
			// ------------------------------
			renderingSystem->SystemRunning();

			// ------------------------------
			//  Run UI system
			// ------------------------------
			uiSystem->SystemRunning();

			// ------------------------------
			//  Run physics system
			// ------------------------------
			physicsSystem->SystemRunning();

			// ------------------------------
			// TODO :: Run Audio system here
			// ------------------------------

			// ------------------------------
			//  Run script system
			// ------------------------------
			scriptingSystem->SystemRunning();


			eventHandler->HandleEvent();

			// ------------------------------
			// On Game Engine
			// ------------------------------
			editor->OnRun();

			guiManager->OnRun();
			// ------------------------------
			// ------------------------------

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
		delete(scriptingSystem);
		delete(physicsSystem);
		delete(uiSystem);
	}

	void Mainframe::Close()
	{
		glfwTerminate();
	}
}