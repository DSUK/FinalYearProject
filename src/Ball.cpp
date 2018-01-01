#include "Ball.h"
#include "Vec.h"
#include <GL/gl.h>

void Ball::draw(GLfloat rad)
{
	float modelview[16];
	GLint Program;
	glTranslatef(pos.x,pos.y,pos.z);
	glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	glUniformMatrix4fv(glGetUniformLocation(Program,"MV"),1,GL_FALSE,modelview);

	GLUquadricObj* Sphere = gluNewQuadric();
	gluSphere(Sphere,rad,20,20);
	gluDeleteQuadric(Sphere);
	glTranslatef(-pos.x,-pos.y,-pos.z);
}

Ball::Ball(Vec position,Vec velocity)
{
	pos = position;
	vel = velocity;
	collide = false;
}
