#include <list>
#include "Ball.h"
#include "Surface.h"

class ThrowContainer
{
	std::list<Ball> objectList;
	public:
	ThrowContainer() { }

	void drawObjects(GLfloat rad);
	void moveObjects(GLfloat delta_time,GLfloat movement,Surface *surface);
	void cullLow();
	void addToList(Vec vel, Vec pos);
};

