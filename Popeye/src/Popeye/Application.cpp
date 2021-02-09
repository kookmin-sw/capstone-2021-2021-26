#include "pch.h"
#include "Application.h"
#include "Mainframe.h"

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
		Popeye::Mainframe* mainframe = new Mainframe();

		if (!mainframe->init_Display())
		{
			delete(mainframe);
			POPEYE_CORE_ERROR("Renderer Initialize fail.");
			return;
		}

		mainframe->run_Display();


		mainframe->close_Display();
		delete mainframe;
		return;
	}

}
