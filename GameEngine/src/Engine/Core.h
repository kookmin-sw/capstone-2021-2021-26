#pragma once

#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif // DEBUG
#else
	#error ENGINE IS ONLY FOR WINDOWS!
#endif // EGINE_PLATFORM_WINDOWS
