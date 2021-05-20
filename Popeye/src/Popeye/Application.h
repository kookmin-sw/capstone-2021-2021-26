#pragma once

#include "Core.h"

namespace Popeye {
	class POPEYE_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
}

