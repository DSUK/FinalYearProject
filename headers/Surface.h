#pragma once

#include "Vec.h"

#include <GL/glew.h>

class Surface {
	struct vert {

		GLfloat div_h;
		GLfloat ddiv_x;
		GLfloat ddiv_y;
		GLfloat ddiv_h;
		Vec pos;
		Vec normal;
	};
	enum fillmode {
		LINE,
		FILL
	};
	fillmode fill;
	bool rain; //fixes flat plane bumping problem
	GLfloat time;
	GLint width;
	GLint length;
	GLfloat dist; //distance each of the vertacies are apart from each other
	GLfloat ddist;
	vert *plane;
	public:
	Surface(GLint x, GLint y, GLfloat distance);
	~Surface();
	void zeroAll();
	void calculateSurfaceNormals();
	void addToNormal(int x, int y, Vec input);
	void setPosition(int x, int y, Vec input);
	void drawSurface(GLint shaderprogram); //renders new surface
	vert accessVert(int x, int y);
	void setHeights();
	void dDivXsurface();
	void dDivYsurface();
	void calculateSurfaceHeight();
	void setTime(GLfloat input);
	void throwStone(int i, int j);
	void switchFillMode();
	void rainDrop(int x, int y);
	void rainSwitch();
	void setHeight(int x, int y, GLfloat value);
	void setDivh(int x, int y, GLfloat value);
	void setDDivx(int x, int y, GLfloat value);
	void setDDivy(int x, int y, GLfloat value);
	void setDDivh(int x, int y, GLfloat value);
	void ballLand(GLfloat x, GLfloat y, GLfloat rad);
};
