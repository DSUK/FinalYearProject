workspace "SurfaceProgram"
	configurations { "Debug", "Release" }
	configuration "vs*"
		platforms{ "Win64" }
	
	configuration "gmake"
		platforms{ "Linux", "Linux64" }

	filter { "platforms:Win64" }
		system "Windows"
		architecture "x64"
	
	filter { "platforms:Linux" }
		system "Linux"
		architecture "x32"
	
	filter { "platforms:Linux64" }
		system "Linux"
		architecture "x64"


	project "SurfaceProgram"
		kind "ConsoleApp"
		language "C++"
		files { "src/*.cpp" }
		includedirs { "headers" }
		links { "SDL"}

		configuration "gmake" 
			buildoptions{ "-std=c++11", "-Wall", "-Werror", "-msse3", "-march=core2" }
			links { "pthread", "GL", "GLEW", "GLU" }
		
		configuration "vs*"
			links { "SDLmain", "opengl32", "glew32", "glu32" }

