workspace "GameEngine"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "GameEngine"
	location "GameEngine"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" ..outputdir.. "/%{prj.name}")
	objdir("bin-int/" ..outputdir.. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "GameEngine/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"GameEngine/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS",
			"ENGINE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Game")
		}
	filter "configurations:Debug"
		defines	"EGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"

project "Game"
	location "Game"
	kind "ConsoleApp"

	language "C++"

	targetdir ("bin/".. outputdir.."/%{prj.name}")
	objdir ("bin-int/".. outputdir.."/%{prj.name}")
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src"
	}

	links
	{
		"GameEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"ENGINE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines	"EGINE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "ENGINE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "ENGINE_DIST"
		optimize "On"