workspace "SurfaceProgram"
	configurations { "Debug", "Release" }
	configuration "vs*"
		platforms{ "Win64" }
	
	filter { "platforms:Win64" }
		system "Windows"
		architecture "x64"
	
	project "SurfaceProgram"
		kind "ConsoleApp"
		language "C++"
		files { "src/*.cpp" }
		includedirs { "headers" }
		links { "SDL2"}

		configuration "gmake" 
			buildoptions{ "-std=c++11", "-Wall", "-Werror", "-msse3", "-march=core2" }
			links { "pthread", "GL", "GLEW", "GLU" }
		
		configuration "vs*"
			links { "SDL2main", "opengl32", "glew32", "glu32" }

