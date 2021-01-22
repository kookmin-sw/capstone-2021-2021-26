#pragma once

#ifdef POPEYE_PLATFORM_WINDOWS
	#ifdef POPEYE_BUILD_DLL
		#define POPEYE_API __declspec(dllexport)
	#else
		#define POPEYE_API __declspec(dllimport)
	#endif // DEBUG
#else
	#error POPEYE IS ONLY FOR WINDOWS!
#endif // EGINE_PLATFORM_WINDOWS
