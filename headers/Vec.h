#pragma once
#include <GL/glew.h>
#define NO_SDL_GLEXT
#ifndef _MSC_VER
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif
#include <xmmintrin.h> //libry for SSE extention
#include <pmmintrin.h> //library for more SSE

struct Vec
{
	union {
		__m128 data;
		struct {
			GLfloat w,x,y,z; //4 floating points now have to be used
		} pos;
	};
	Vec();
	Vec(__m128 input);
	Vec(GLfloat ix, GLfloat iy, GLfloat iz);
	void normalise();
	Vec toUnit();
	void operator = (Vec rhs);
	void operator += (Vec rhs);
};

Vec operator + (Vec lhs, Vec rhs);
Vec operator - (Vec rhs);
Vec operator - (Vec lhs, Vec rhs);
Vec operator % (Vec lhs, Vec rhs); //cross product 
Vec operator * (GLfloat lhs,Vec rhs);
Vec operator / (Vec lhs, GLfloat rhs);
