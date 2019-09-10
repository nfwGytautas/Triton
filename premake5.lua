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
IncludeDir["ImGui"] = "vendor/imguiDocking"
IncludeDir["Assimp"] = "vendor/Assimp/include"
IncludeDir["stb_image"] = "vendor/stb_image"
IncludeDir["cereal"] = "vendor/cereal/include"
IncludeDir["XTK"] = "vendor/DirectXTK/Inc"
IncludeDir["mono"] = "vendor/mono/include/mono-2.0"

include "vendor/GLFW"
include "vendor/Glad"
include "vendor/imguiDocking"


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
		"%{IncludeDir.XTK}",
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
		"%{IncludeDir.XTK}",
		"%{IncludeDir.mono}",
		"TritonGraphics/src",
	}

	libdirs
	{
		"C:/dev/Triton/vendor/Assimp/lib/Release",
		"C:/dev/Triton/vendor/mono/lib",
	}
	
	links 
	{ 
		"TritonGraphics",
		"assimp-vc140-mt.lib",
		"mono-2.0-sgen.lib",
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TR_PLATFORM_WINDOWS",
			"NOMINMAX",
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

project "TritonEditor"
	location "TritonEditor"
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
		"%{prj.name}/src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.XTK}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.cereal}",
		"TritonEditor/src",
		"TritonCore/src",
		"TritonGraphics/src",
	}

	links
	{
		"TritonCore",
		"ImGui",
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

project "TritonEngine"
	location "TritonEngine"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.cs"
	}

	links
	{
		"System",
	}
	
	postbuildcommands 
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin"),
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

project "TritonEditorExtension"
	location "TritonEditorExtension"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	links
	{
		"System",
		"TritonEngine"
	}

	files
	{
		"%{prj.name}/src/**.cs"
	}
	
	postbuildcommands 
	{
		("{COPY} %{cfg.buildtarget.relpath} ../"),
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
		"%{IncludeDir.XTK}",
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