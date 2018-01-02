#pragma once
#include <GL/glew.h>
#define NO_SDL_GLEXT
#ifndef _MSC_VER
#include <SDL2/SDL_opengl.h>
#else
#include <SDL_opengl.h>
#endif
#include <xmmintrin.h>
#include <pmmintrin.h>

struct Vec
{
	union {
		__m128 data;
		struct {
			GLfloat x,y,z,w;
		};
		GLfloat _arr[4];
	};
	Vec();
	Vec(__m128 input);
	Vec(GLfloat ix, GLfloat iy, GLfloat iz);
	Vec(GLfloat ix, GLfloat iy, GLfloat iz, GLfloat iw);
	void normalise();
	Vec toUnit();
	void operator = (Vec rhs);
	void operator += (Vec rhs);
	GLfloat operator* (Vec other);
	GLfloat& operator[] (GLuint index);
};

Vec operator + (Vec lhs, Vec rhs);
Vec operator - (Vec rhs);
Vec operator - (Vec lhs, Vec rhs);
Vec operator % (Vec lhs, Vec rhs); //cross product 
Vec operator * (GLfloat lhs,Vec rhs);
Vec operator / (Vec lhs, GLfloat rhs);
