#include "pch.h"
#include "Application.h"
#include "Renderer.h"

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
		Popeye::Renderer* renderer = new Renderer();

		if (!renderer->init_renderer())
		{
			delete(renderer);
			POPEYE_CORE_ERROR("Renderer Initialize fail.");
			return;
		}

		renderer->run_renderer();


		renderer->close_renderer();
		delete renderer;
		return;
	}

}
