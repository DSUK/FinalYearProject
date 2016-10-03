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

F7 -> reset camera position

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

the project uses premake5, SDL1.2, GLU, GLEW and opengl

    $sudo apt-get install libglu1-mesa libglu1-mesa-dev libgl1-mesa-glx libgl1-mesa-dev libsdl1.2-dev libglu-dev libglew-dev 

also needs [premake5](http://premake.github.io/download.html) installed

    $premake5 gmake
    $make

###Windows

TBC
