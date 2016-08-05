workspace "SurfaceProgram"
	configurations { "Debug", "Release" }

project "SurfaceProgram"
	kind "ConsoleApp"
	language "C++"
	files { "src/*.cpp" }
	includedirs { "headers" }
	links { "SDL", "GLEW", "GL", "pthread", "GLU" }

--TODO: create a directory with root modules for dependencies
configuration { "gmake" }
	buildoptions{ "-std=c++11", "-Wall", "-Werror", "-msse3", "-march=core2" }
