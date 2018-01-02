#pragma once
#include "Vec.h"

const GLfloat kFovy = 0.75f;
const GLfloat kPerspectiveNear = 1.f;
const GLfloat kPersperciveFar = 700.f;

class Mat4x4
{
public:
	union {
		Vec vecs[4];
		struct {
			Vec x_t;
			Vec y_t;
			Vec z_t;
			Vec w_t;
		};
	};
	GLfloat& operator()(int, int);
	Mat4x4(Vec x_t, Vec y_t, Vec z_t, Vec w_t);
	static Mat4x4 CreateIdentity();
	Vec operator* (Vec vec);
	Mat4x4 operator* (Mat4x4 other);
	static void RotateY(Mat4x4 &mat, GLfloat angle);
	static void RotateVec(Mat4x4 &mat, Vec dir, GLfloat angle);
	static void Translate(Mat4x4 &mat, Vec translate);
	static Mat4x4 Perspective(GLfloat fov, GLfloat aspect, GLfloat nearz, GLfloat farz);
};
static_assert(sizeof(Mat4x4) == 64, "4x4 matrix must be 512bit in size");