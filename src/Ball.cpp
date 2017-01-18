#include "Ball.h"
#include "Vec.h"
#include <GL/gl.h>

void Ball::draw(GLfloat rad)
{
	glTranslatef(pos.pos.x,pos.pos.y,pos.pos.z);
	GLUquadricObj* Sphere = gluNewQuadric();
	gluSphere(Sphere,rad,20,20);
	gluDeleteQuadric(Sphere);
	glTranslatef(-pos.pos.x,-pos.pos.y,-pos.pos.z);
}

Ball::Ball(Vec position,Vec velocity)
{
	pos = position;
	vel = velocity;
	collide = false;
}
