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

		if (!mainframe->Init())
		{
			delete(mainframe);
			POPEYE_CORE_ERROR("Fail to Initialze.");
			return;
		}

		mainframe->Run();


		mainframe->Close();
		delete mainframe;
		return;
	}

}
