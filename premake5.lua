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
IncludeDir["Assimp"] = "vendor/Assimp/include"
IncludeDir["stb_image"] = "vendor/stb_image"
IncludeDir["cereal"] = "vendor/cereal/include"
IncludeDir["XTK"] = "vendor/DirectXTK/Inc"
IncludeDir["mono"] = "vendor/mono/include/mono-2.0"
IncludeDir["QT"] = "vendor/QT/include/mono-2.0"
IncludeDir["spdlog"] = "vendor/spdlog/include"
IncludeDir["FreeType"] = "vendor/FreeType/include"

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
		"vendor/GLFW/bin/" .. outputdir .. "/GLFW/GLFW.lib",
		"vendor/Glad/bin/" .. outputdir .. "/Glad/Glad.lib",
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
		"%{IncludeDir.FreeType}",
		"TritonGraphics/src",
	}

	libdirs
	{
		"vendor/Assimp/lib/Release",
		"vendor/mono/lib",
		"vendor/FreeType/win64",
	}
	
	links 
	{ 
		"TritonGraphics",
		"assimp-vc140-mt.lib",
		"mono-2.0-sgen.lib",
		"freetype.lib",
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

	postbuildcommands {
		("{COPY} ../vendor/FreeType/win64/freetype.dll ../bin/" .. outputdir .. "/%{prj.name}")
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