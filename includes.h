#ifndef _PROGRAM_HEAD
#define _PROGRAM_HEAD
//#include <GL/freeglut.h>
//#define GL_GLEXT_PROTOTYPES
#include <GL/glew.h>

#define NO_SDL_GLEXT
#include <SDL/SDL.h> //take that, glut
#include <SDL/SDL_opengl.h>

//#include <cstdio>
#include <GL/glu.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include "vec.cpp"
#include "MouseClass.cpp"
#include "SurfaceClass.cpp"
void handleresize(int,int);
void programloop();
#endif
