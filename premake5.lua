workspace "Popeye"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir = {}
IncludeDir["GLFW"] = "Popeye/vendor/GLFW/include"
IncludeDir["glad"] = "Popeye/vendor/glad/include"
IncludeDir["ImGUI"] = "Popeye/vendor/imgui"

include "Popeye/vendor/GLFW"
include "Popeye/vendor/glad"
include "Popeye/vendor/imgui"

project "Popeye"
	location "Popeye"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" ..outputdir.. "/%{prj.name}")
	objdir("bin-int/" ..outputdir.. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Popeye/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glm",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.ImGUI}",
		"Popeye/src"
	}

	links
	{
		"GLFW",
		"glad",
		"ImGUI",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"POPEYE_PLATFORM_WINDOWS",
			"POPEYE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" ..outputdir.. "/Eyeball")
		}
	filter "configurations:Debug"
		defines	"POPEYE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "POPEYE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "POPEYE_DIST"
		optimize "On"

project "Eyeball"
	location "Eyeball"
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
		"Popeye/vendor/spdlog/include",
		"Popeye/src"
	}

	links
	{
		"Popeye"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"POPEYE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines	"POPEYE_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "POPEYE_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "POPEYE_DIST"
		optimize "On"