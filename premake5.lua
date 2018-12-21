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
IncludeDir["GLFW"] = "Triton/vendor/GLFW/include"
IncludeDir["GLM"] = "Triton/vendor/glm"

include "Triton/vendor/GLFW"

project "Triton"
	location "Triton"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "TRpch.h"
	pchsource "Triton/src/TRpch.cpp"

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
		"%{IncludeDir.GLM}",		
		"%{prj.name}/vendor/glew/include",	
	}
	
	libdirs 
	{ 
		"%{prj.name}/vendor/glew/",
	}
	
	links 
	{ 
		"glew32",
		"GLFW" ,
		"opengl32.lib"
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
			("{COPY} vendor/glew/glew32.dll ../bin/" .. outputdir .. "/SandBox"),
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
		"Triton/vendor/spdlog/include",
		"Triton/vendor/glew/include",
		"%{IncludeDir.GLM}",
		"Triton/src"
	}

	links
	{
		"Triton"
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