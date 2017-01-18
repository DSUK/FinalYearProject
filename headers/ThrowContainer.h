#include <list>
#include "Ball.h"
#include "Surface.h"

class ThrowContainer
{
	std::list<Ball> objectList;
	public:
	ThrowContainer() { }

	void drawobjects(GLfloat rad);
	void moveobjects(GLfloat delta_time,GLfloat movement,Surface *surface);
	void cullLow();
	void addtolist(Vec vel, Vec pos);
};

