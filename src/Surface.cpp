#include "Vec.h"

#define DAMP 0.999
#define NO_SDL_GLEXT
#include <cstdlib>
#include <ctime>
#include <cmath>
#ifndef _MSC_VER
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#else
#include <SDL_opengl.h>
#include <SDL.h>
#endif

#include "Surface.h"

float sinc(float input) {
	if(input == 0.0) {
		return 1.0;
	}
	return sin(input)/input;
}

Surface::Surface(GLint x, GLint y, GLfloat distance) {

	time = 0.005;
	width = x;
	length = y;
	dist = distance;
	ddist = distance*distance;
	plane = new vert[width*length]; //allocate memory for the plane
	vertecies = new Vec[width*length]; //allocate memory for the plane
	normals = new Vec[width*length];
	rain = false;
	fill = GL_LINE;
	srand(time);
	for(int i = 0; i < x; ++i)
	{
	    for(int j = 0; j < y; ++j)
	    {
			setPosition(i,j,Vec(((GLfloat)i)*distance,0,((GLfloat)j)*distance));
	    }
	}
	zeroAll();
	//glUseProgram(0);
}

Surface::~Surface() {
	delete[] plane;
	delete[] vertecies;
	delete[] normals;
}

void Surface::zeroAll() {
	for(int i = 0; i < width*length; ++i) {
		vertecies[i].pos.y = 0.0;
		plane[i].div_h = 0.0;
		plane[i].ddiv_x = 0.0;
		plane[i].ddiv_y = 0.0;
		plane[i].ddiv_h = 0.0;
	}
}

void Surface::calculateSurfaceNormals() {
	//zero it all first
	for(int x  = 0; x < width*length; ++x) {
		normals[x] = Vec(0,0,0);
	}
	//- d_x % d_y
	for(int x = 0; x < width-1; ++x) {
		for(int y = 0; y < length-1; ++y) {
			addToNormal(x,y,((access2D(x,y,vertecies) - access2D(x+1,y,vertecies))%
				(access2D(x,y+1,vertecies)) - access2D(x,y,vertecies)).toUnit());
		}
	}
	//- d_y % -d_x
	for(int x = 1; x < width; ++x) {
		for(int y = 0; y < length-1; ++y) {
			addToNormal(x,y,((access2D(x,y,vertecies) - access2D(x,y+1,vertecies))%
				(access2D(x-1,y,vertecies) - access2D(x,y,vertecies))).toUnit());
		}
	}
	//d_x % -d_y
	for(int x = 1; x < width; ++x) {
		for(int y = 1; y < length; ++y) {
			addToNormal(x,y,((access2D(x,y,vertecies) - access2D(x-1,y,vertecies)%
				(access2D(x,y-1,vertecies) - access2D(x,y,vertecies))).toUnit()));
		}
	}
	//d_y % d_x
	for(int x = 0; x < width-1; ++x) {
		for(int y = 1; y < length; ++y) {
			addToNormal(x,y,((access2D(x,y,vertecies) - access2D(x,y-1,vertecies))%
				(access2D(x+1,y,vertecies) - access2D(x,y,vertecies))).toUnit());
		}
	}
}

void Surface::addToNormal(int x, int y, Vec input) {
	SDL_assert(x < width && y < length && x >= 0 && y >= 0);
    normals[x+y*width] += input;
}

void Surface::setPosition(int x, int y, Vec input) {
	SDL_assert(x < width && y < length && x >= 0 && y >= 0);
    vertecies[x+y*width] = input;
}

void Surface::drawSurface(GLint shaderprogram) {
	glUniform3f(glGetUniformLocation(shaderprogram,"samb"),0.1,0.1,0.5);
	glUniform3f(glGetUniformLocation(shaderprogram,"sdif"),0.1,0.1,0.9);
	glUniform3f(glGetUniformLocation(shaderprogram,"sspec"),1.0,1.0,1.0);
	glUniform1f(glGetUniformLocation(shaderprogram,"ref_number"),30.0);
	for(int x = 0; x < width-1; ++x) {
		glBegin(GL_TRIANGLE_STRIP);
		for(int y = 0; y < length; ++y) {
			Vec posi = access2D(x,y,vertecies);
			Vec norm = access2D(x,y,normals);
			glVertex3f(posi.pos.x,posi.pos.y,posi.pos.z);
			glNormal3f(norm.pos.x,norm.pos.y,norm.pos.z);
			posi = access2D(x+1,y,vertecies);
			norm = access2D(x+1,y,normals);
			glVertex3f(posi.pos.x,posi.pos.y,posi.pos.z);
			glNormal3f(norm.pos.x,norm.pos.y,norm.pos.z);
		}
		glEnd();
	}
}

Surface::vert Surface::accessVert(int x, int y) {
	SDL_assert(x < width && y < length && x >= 0 && y >= 0);
	return plane[x+y*width];
}


void Surface::setHeights() {
	dDivXsurface();
	dDivYsurface();
	calculateSurfaceHeight();
	if(rain && (rand()&3) == 3) {
		rainDrop(rand()%width, rand()%length);
	}
}

void Surface::dDivXsurface() {
		//0 edge
	for(int i = 0; i < length; ++i) {
		setDDivx(0,i,(access2D(2,i,vertecies).pos.y + access2D(1,i,vertecies).pos.y -
			2*access2D(0,i,vertecies).pos.y)/ddist);
	}
	//width edge (at width -1)
	for(int i = 0; i<length; ++i) {
		setDDivx(width-1,i,(access2D(width-3,i,vertecies).pos.y + access2D(width-2,i,vertecies).pos.y -
			2*access2D(width-1,i,vertecies).pos.y)/(ddist));
	}

	for(int i = 0; i < length; ++i)
		for(int j = 1; j < width-1; ++j) {
			setDDivx(j,i,(access2D(j-1,i,vertecies).pos.y + access2D(j+1,i,vertecies).pos.y -
				2*access2D(j,i,vertecies).pos.y)/ddist);
	}
}

void Surface::dDivYsurface() {
	//0 edge
	for(int i = 0; i < width; ++i) {
		setDDivy(i,0,(access2D(i,2,vertecies).pos.y + access2D(i,1,vertecies).pos.y -
			2*access2D(i,0,vertecies).pos.y)/ddist);
	}

	for(int i = 0; i<width; ++i) {
		setDDivy(i,length-1,(access2D(i,length-2,vertecies).pos.y + access2D(i,length-3,vertecies).pos.y -
			2*access2D(i,length-1,vertecies).pos.y )/ddist); //backwards
	}

	for(int i = 0; i < width; ++i)
		for(int j = 1; j < length-1; ++j) {
			setDDivy(i,j,(access2D(i,j-1,vertecies).pos.y + access2D(i,j+1,vertecies).pos.y -
				2*access2D(i,j,vertecies).pos.y)/ddist);
	}
}

void Surface::calculateSurfaceHeight() {
	//wave equation
	for(int i = 0; i< length*width; ++i) {
		plane[i].ddiv_h = plane[i].ddiv_x + plane[i].ddiv_y;
		plane[i].div_h += time*plane[i].ddiv_h;
		vertecies[i].pos.y = vertecies[i].pos.y*DAMP + time*plane[i].div_h;
	}
}

void Surface::setTime(GLfloat input) {
	time = input;
}

void Surface::throwStone(int i, int j) {
	setDivh(i,j,-10.0);
}

void Surface::switchFillMode() {
	fill ^= GL_FILL^GL_LINE;
	glPolygonMode(GL_FRONT_AND_BACK, fill);
}

void Surface::rainDrop(int x, int y) {
	setDivh(x,y,accessVert(x,y).div_h -0.5);
}
void Surface::rainSwitch() {
    rain = !rain;
}
void Surface::setHeight(int x, int y, GLfloat value) {
	if((x < width) & (y < length) & (x >= 0) & (y >= 0)) {
		vertecies[x+y*width].pos.y = value;
	}
}
void Surface::setDivh(int x, int y, GLfloat value) {
	if((x < width) & (y < length) & (x >= 0) & (y >= 0)) {
		plane[x+y*width].div_h = value;
	}
}
void Surface::setDDivx(int x, int y, GLfloat value) {
	if((x < width) & (y < length) & (x >= 0) & (y >= 0)) {
		plane[x+y*width].ddiv_x = value;
	}
}
void Surface::setDDivy(int x, int y, GLfloat value) {
	if((x < width) & (y < length) & (x >= 0) & (y >= 0)) {
		plane[x+y*width].ddiv_y = value;
	}
}
void Surface::setDDivh(int x, int y, GLfloat value){
	if((x < width) & (y < length) & (x >= 0) & (y >= 0)) {
		plane[x+y*width].ddiv_h = value;
	}
}

void Surface::ballLand(GLfloat x, GLfloat y) {
	if((x > 0) & (y > 0) & (y < length*dist) & (x < width*dist)) { //calculate widths
	    GLfloat xmod = fmod(x,dist);
	    GLfloat ymod = fmod(y,dist);
	    GLint intx = (GLint)(x/dist);
	    GLint inty = (GLint)(y/dist);
	    intx += (xmod > dist/2);
	    inty += (ymod > dist/2);
	    throwStone(intx,inty);
	}
}

