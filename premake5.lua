workspace "Triton"   
	architecture "x64"

	configurations 
	{ 
		"Debug", 
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["GLM"] = "vendor/glm"
IncludeDir["entt"] = "vendor/entt/src"
IncludeDir["pybind"] = "vendor/pybind/include"
IncludeDir["python0"] = "vendor/python/include"
IncludeDir["python1"] = "vendor/python/PC"
IncludeDir["ImGui"] = "vendor/ImGui"
IncludeDir["Assimp"] = "vendor/Assimp/include"
IncludeDir["stb_image"] = "vendor/stb_image"
IncludeDir["cereal"] = "vendor/cereal/include"

include "vendor/GLFW"
include "vendor/Glad"
include "vendor/ImGui"


project "TritonGraphics"
	location "TritonGraphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"TritonCore/src",
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE",
		}
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "TR_DIST"
		optimize "on"
		runtime "Release"


project "TritonCore"
	location "TritonCore"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "TRpch.h"
	pchsource "TritonCore/src/TRpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
		"%{IncludeDir.cereal}",
		"TritonGraphics/src",
	}

	libdirs
	{
		"C:/dev/Triton/vendor/Assimp/lib/Release"
	}
	
	links 
	{ 
		"TritonGraphics",
		"assimp-vc140-mt.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS",
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "TR_DIST"
		optimize "on"
		runtime "Release"


project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"	
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.cereal}",
		"TritonCore/src",
		"TritonGraphics/src",
	}

	links
	{
		"TritonCore",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS"
		}
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "on"
		runtime "Release"

	filter "configurations:Dist"
		defines "TR_DIST"
		runtime "Release"
		optimize "on"
