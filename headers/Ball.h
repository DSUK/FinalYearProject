#pragma once
#include "Vec.h"
#include <GL/gl.h>

struct Ball
{
	Vec pos;
	Vec vel;
	bool collide;
	Ball(Vec position,Vec velocity);
	void draw(GLfloat rad);
};
