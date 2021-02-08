#pragma once

#ifdef POPEYE_PLATFORM_WINDOWS

extern Popeye::Application* Popeye::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Popeye::CreateApplication();
	app->Run();
	delete app;
}

#endif // ENGINE_PLATFORM_WINDOWS
