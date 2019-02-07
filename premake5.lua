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
IncludeDir["GLFW"] = "TritonCore/vendor/GLFW/include"
IncludeDir["Glad"] = "TritonCore/vendor/Glad/include"
IncludeDir["GLM"] = "TritonCore/vendor/glm"
IncludeDir["entt"] = "TritonCore/vendor/entt/src"
IncludeDir["pybind"] = "TritonScript/vendor/pybind/include"
IncludeDir["python0"] = "TritonScript/vendor/python/include"
IncludeDir["python1"] = "TritonScript/vendor/python/PC"

include "TritonCore/vendor/GLFW"
include "TritonCore/vendor/Glad"

project "TritonScript"
	location "TritonScript"
	kind "SharedLib"
	language "C++"

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
		"TritonCore/vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.python0}",
		"%{IncludeDir.python1}",
		"TritonCore/src",
	}
	
	libdirs
	{
		"TritonScript/vendor/python/PCbuild/amd64/",
		"TritonScript/../bin/" .. outputdir .. "/TritonCore/"
	}

	links 
	{ 
		"TritonCore",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS",
			"TR_SCRIPTING_BUILD_DLL",
			"_WINDLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox"),
			("{COPY} %{prj.location}/vendor/python/PCbuild/amd64/python38.dll ../bin/" .. outputdir .. "/SandBox"),					
			("{COPY} %{prj.location}/vendor/python/Lib/ ../bin/" .. outputdir .. "/SandBox/Lib"),
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS",
		}
		symbols "On"
		staticruntime "off"
	
	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "On"
		staticruntime "off"

	filter "configurations:Dist"
		defines "TR_DIST"
		optimize "On"
		staticruntime "off"

project "TritonCore"
	location "TritonCore"
	kind "SharedLib"
	language "C++"

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
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{prj.name}/vendor/stb_image/include",
	}

	libdirs
	{
		"TritonScript/vendor/python/PCbuild/amd64/",
	}
	
	links 
	{ 
		"GLFW",
		"Glad",
		"opengl32.lib",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS",
			"TR_BUILD_DLL",
			"_WINDLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox"),
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TritonScript"),
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS",
			"GLFW_INCLUDE_NONE",
		}
		symbols "On"
		staticruntime "off"
	
	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "On"
		staticruntime "off"

	filter "configurations:Dist"
		defines "TR_DIST"
		optimize "On"
		staticruntime "off"

project "SandBox"
	location "SandBox"
	kind "ConsoleApp"
	language "C++"	

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"TritonCore/vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.python0}",
		"%{IncludeDir.python1}",
		"TritonCore/src",
		"TritonScript/src"
	}
	
	libdirs
	{
		"TritonScript/vendor/python/PCbuild/amd64/",
	}

	links
	{
		"TritonCore",
		"TritonScript",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines 
		{
			"TR_DEBUG",
			"TR_ENABLE_ASSERTS",
		}
		symbols "On"
		staticruntime "off"
	
	filter "configurations:Release"
		defines "TR_RELEASE"
		optimize "On"
		staticruntime "off"

	filter "configurations:Dist"
		defines "TR_DIST"
		optimize "On"
		staticruntime "off"