-- premake5.lua
workspace "UI"
	configurations {"Debug", "Release"}
	location "workspace"
	
project "UI"
	kind "ConsoleApp"
	language "C++"
	targetdir "./build/bin/%{cfg.buildcfg}"
	location "workspace/UI"
	includedirs "include"
	links { "OpenGL32", "glew", "glfw3" }
	defines { "NANOVG_GLEW" }
	debugdir "./bin"
	
	files { "./src/*.h", "./src/*.cpp" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		libdirs { "./lib/nanovg/win_32/debug" }
		links { "nanovg" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		libdirs { "./lib/nanovg/win_32/release" }
		links { "nanovg" }
		