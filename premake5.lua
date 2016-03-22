-- premake5.lua
workspace "UI"
	configurations {"Debug", "Release"}
	location "workspace"
	
project "UILib"
	kind "StaticLib"
	language "C++"
	targetdir "./build/lib/%{cfg.buildcfg}"
	location "workspace/lib"
	includedirs {"include", "src/lib", "src/lib/**"}
	links { "OpenGL32", "glew", "glfw3" }
	defines { "NANOVG_GLEW" }
	debugdir "./bin"
	
	files { "./src/lib/**.h", "./src/lib/**.cpp" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		libdirs { "./lib/nanovg/win_32/debug", "./lib/pugixml/win_32/debug" }
		links { "nanovg", "pugixml" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		libdirs { "./lib/nanovg/win_32/release", "./lib/pugixml/win_32/release" }
		links { "nanovg", "pugixml" }
	
project "DemoApp"
	kind "ConsoleApp"
	language "C++"
	targetdir "./build/bin/%{cfg.buildcfg}"
	location "workspace/DemoApp"
	includedirs "include"
	links { "OpenGL32", "glew", "glfw3" }
	debugdir "./bin"
	
	files { "./src/demo_app/*.h", "./src/demo_app/*.cpp" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		libdirs { "./lib/nanovg/win_32/debug", "./build/lib/debug" }
		links { "nanovg" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		libdirs { "./lib/nanovg/win_32/release", "./build/lib/release" }
		links { "nanovg" }
		
		
project "UIApp"
	kind "ConsoleApp"
	language "C++"
	targetdir "./build/bin/%{cfg.buildcfg}"
	location "workspace/DemoApp"
	includedirs "include"
	links { "OpenGL32", "glew", "glfw3" }
	debugdir "./bin"
	
	files { "./src/ui_app/*.h", "./src/ui_app/*.cpp" }
	
	filter "configurations:Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		libdirs { "./lib/nanovg/win_32/debug", "./build/lib/debug", "./lib/pugixml/win_32/debug" }
		links { "nanovg", "pugixml", "UILib" }
		
	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "On"
		libdirs { "./lib/nanovg/win_32/release", "./build/lib/release", "./lib/pugixml/win_32/release" }
		links { "nanovg", "pugixml", "UILib" }