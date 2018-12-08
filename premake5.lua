workspace "Triton"   
   architecture "x64"

   configurations 
   { 
		"Debug", 
		"Release",
		"Dist"
   }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Triton"
	location "Triton"
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
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/glew/include",
	}
	
	libdirs 
	{ 
		"%{prj.name}/vendor/glew/",
		"%{prj.name}/vendor/glfw/"
	}
	
	links 
	{ 
		"glew32",
		"glfw3" ,
		"opengl32"
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
			("{COPY} vendor/glfw/glfw3.dll ../bin/" .. outputdir .. "/SandBox")
		}

	filter "configurations:Debug"
		defines "HZ_DEBUG"
		symbols "On"
		staticruntime "off"
	
	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"
		staticruntime "off"

	filter "configurations:Dist"
		defines "HZ_DIST"
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
		"Triton/vendor/glfw/include",
		"Triton/vendor/glew/include",
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
		defines "HZ_DEBUG"
		symbols "On"
		staticruntime "off"
	
	filter "configurations:Release"
		defines "HZ_RELEASE"
		optimize "On"
		staticruntime "off"

	filter "configurations:Dist"
		defines "HZ_DIST"
		optimize "On"
		staticruntime "off"