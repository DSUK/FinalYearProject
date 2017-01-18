#include "ThrowContainer.h"
#include "Surface.h"
#include "Ball.h"


void ThrowContainer::drawobjects(GLfloat rad)
{
		for(auto iter : objectList)
		{
			iter.draw(rad);
		}
}
void ThrowContainer::moveobjects(GLfloat delta_time,GLfloat movement,Surface *surface)
{
	for(std::list<Ball>::iterator iter = objectList.begin(); iter != objectList.end(); ++iter)
	{
		iter->vel.pos.y -= movement;
		iter-> pos = iter->pos + delta_time*iter->vel;
		if(iter->collide == false && iter->pos.pos.y <= 0)
		{
			iter->collide = true;
			surface->ballLand(iter->pos.pos.x,iter->pos.pos.z,0.0);
		}
	}
}
void ThrowContainer::cullLow()
{
	for(auto iter = objectList.begin(); iter != objectList.end(); ++iter)
	{
		if(iter->pos.pos.y < -1)
		{
			iter = objectList.erase(iter);
		}
	}
}
void ThrowContainer::addtolist(Vec vel, Vec pos)
{
	objectList.push_back(Ball(pos,vel));
}
