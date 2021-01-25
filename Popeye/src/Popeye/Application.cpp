#include "pch.h"
#include "Application.h"
#include "Display.h"

namespace Popeye {
	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		Popeye::Log::Init();
		Popeye::Display* display = new Display();

		if (!display->init_Display())
		{
			delete(display);
			POPEYE_CORE_ERROR("Renderer Initialize fail.");
			return;
		}

		display->run_Display();


		display->close_Display();
		delete display;
		return;
	}

}
