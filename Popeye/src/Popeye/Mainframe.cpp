#include "Mainframe.h"

#include "Manager/GUIManager.h"
#include "Manager/ComponentManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"

#include "FileIO.h"
#include "Event/EventHandler.h"

#include "System/RenderingSystem.h"
#include "System/ScriptingSystem.h"


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
		

		ComponentManager::GetInstance()->InitComponents();

		SceneManager::GetInstance()->CreateScene();

		RenderingSystem* renderingSystem = new RenderingSystem();
		renderingSystem->SystemInit();

		ScriptingSystem* scriptingSystem = new ScriptingSystem();
		scriptingSystem->SystemInit();

		int display_w, display_h;
		while (!glfwWindowShouldClose(window))
		{
			glfwGetFramebufferSize(window, &display_w, &display_h);
			
			// Rendering system
			renderingSystem->SystemRunning();

			// ------------------------------
			// TODO :: Run UI system here
			// ------------------------------

			// ------------------------------
			// TODO :: Run Physic system here
			// ------------------------------

			// ------------------------------
			// TODO :: Run Audio system here
			// ------------------------------

			// Scripting system
			scriptingSystem->SystemRunning();


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
		delete(scriptingSystem);

	}

	void Mainframe::Close()
	{
		glfwTerminate();
	}
}