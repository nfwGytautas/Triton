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

include "vendor/GLFW"
include "vendor/Glad"
include "vendor/ImGui"

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
		"vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.python0}",
		"%{IncludeDir.python1}",
		"%{IncludeDir.entt}",
		"TritonCore/src",
		"TritonShell/src",
	}
	
	libdirs
	{
		"vendor/python/PCbuild/amd64/",
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
			("{COPY} ../vendor/python/PCbuild/amd64/python38.dll ../bin/" .. outputdir .. "/SandBox"),					
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
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.Assimp}",
	}

	libdirs
	{
		"C:/dev/Triton/vendor/Assimp/lib/Release"
	}
	
	links 
	{ 
		"assimp-vc140-mt.lib",
		"GLFW",
		"Glad",
		"ImGui",
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

project "TritonShell"
	location "TritonShell"
	kind "StaticLib"
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
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.python0}",
		"%{IncludeDir.python1}",
		"%{IncludeDir.ImGui}",
		"TritonCore/src",
		"TritonScript/src"
	}

	libdirs
	{
		"vendor/python/PCbuild/amd64/",
	}
	
	links 
	{ 
		"TritonCore",
		"TritonScript",
		"ImGui",
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
		"vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.pybind}",
		"%{IncludeDir.python0}",
		"%{IncludeDir.python1}",
		"%{IncludeDir.ImGui}",
		"TritonCore/src",
		"TritonScript/src",
		"TritonShell/src",
	}
	
	libdirs
	{
		"vendor/python/PCbuild/amd64/",
	}

	links
	{
		"TritonShell",
	}

	postbuildcommands
	{
		("{COPY} %{prj.location}../PythonScripts/*.py ../bin/" .. outputdir .. "/SandBox/Logic"),
		("{COPY} %{prj.location}../PythonScripts/*.pyi ../bin/" .. outputdir .. "/SandBox/Logic"),
		("{COPY} %{prj.location}../PythonScripts/User/*.py ../bin/" .. outputdir .. "/SandBox/Logic/User"),
		("{COPY} %{prj.location}../Configs/triton_config.py ../bin/" .. outputdir .. "/SandBox"),
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