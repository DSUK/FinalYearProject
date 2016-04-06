#include "includes.h"

struct vec
{
	GLfloat x,y,z;

	vec operator + (vec rhs)
	{
        vec temp;
        temp.x = x + rhs.x;
        temp.y = y + rhs.y;
        temp.z = z + rhs.z;
        return temp;
	}
	GLfloat operator * (vec lhs, vec rhs) //dot product
	{
	    return lhs.x*rhs.x + lhs.y*rhs.y + lhs.z*rhs.z;
	}
	vec operator - (vec lhs, vec rhs)
	{
	    return lhs + (-rhs);
	}
	vec operator % (vec lhs, vec rhs)  //cross product
	{
	    vec temp;
	    temp.x = lhs.y*rhs.z - lhs.z*rhs.y;
	    temp.y = lhs.z*rhs.x - lhs.x*rhs.z;
	    temp.z = lhs.x*rhs.y - lhs.y*rhs.x;
	    return temp;
	}
	vec operator * (GLfloat lhs,vec rhs)
	{

	}
	vec toUnit()
	{

	}
};
