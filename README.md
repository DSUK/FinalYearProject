# Final Year Project

This is my final Year project for my BSc in Computational Mathematics.
The title is of the project is "Procedual Generation Of Waves In Water"
The program was orignally designed to target Linux64 and has sinced been ported to windows64 via mingw32.

##Controls:
W S A D/ Arrows - > move

Space - > Capture/Uncapture mouse

Backspace - > flatten surface

r - > random/rain

f - > wireframe on/off

###Mouse (when captured):

move - > move camera

left click - > throw ball

##Dependencies:

OpenGL 2.0

SDL 1.2

GLEW

mingw32 on windows.

##Building:

###Linux

the project uses Code blocks, SDL1.2, GLU, GLEW and opengl

    $sudo apt-get install libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libsdl1.2-dev libglu-dev libglew-dev codeblocks

should download everything you need, just open the .cbp in codeblocks and press the green triangle.

###Windows

Windows Has 3 options, in order of difficulty :

1. Do Nothing:

>I have already included a pre-built version in /SurfaceProgram/bin/Win64

2. Imitate Linux

>Get MSYS2: https://msys2.github.io/ and Codeblocks (without MinGW32): http://www.codeblocks.org/

>open MSYS2 and type:

    pacman -S mingw64/mingw-w64-x86_64-gcc mingw64/mingw-w64-x86_64-SDL mingw64/mingw-w64-x86_64-glew mingw64/mingw-w64-x86_64-make

>open codeblocks and go to Settings > Compiler > ToolChain Executables > set compiler's installation to {msys install}/mgw64/

>click "auto detect"

>"Ok"

>open .cbp file and click play

3. The Hard way

>Get codeblocks with MinGW32 and get SDL from https://www.libsdl.org/download-1.2.php and GLEW from http://glew.sourceforge.net/
GLEW doesn't offically support MinGW32, It is usllay built in a farily hacky way (http://stackoverflow.com/questions/6005076/building-glew-on-windows-with-mingw)
