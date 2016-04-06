#ifndef _SURFACE_CLASS //include guards
#define _SURFACE_CLASS
#include "MouseClass.cpp"
#define DAMP 0.999
#include <cstdlib>
#include <ctime>
void DrawBall(GLfloat,vec);
struct vert
{
	GLfloat div_h;
	GLfloat ddiv_x;
	GLfloat ddiv_y;
	GLfloat ddiv_h;
	vec pos;
    vec normal;
};
class Surface_class
{
	enum fillmode
	{
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
	Surface_class(GLint x, GLint y, GLfloat distance)
	{
		time = 0.005;
		width = x;
		length = y;
		dist = distance;
		ddist = distance*distance;
		plane = new vert[width*length]; //allocate memory for the plane
		rain = false;
		fill = LINE;
		srand(time);
		for(int i = 0; i<x;++i)
		{
		    for(int j = 0; j<y;++j)
		    {
		        setpos(i,j,vec(((GLfloat)i)*distance,0,((GLfloat)j)*distance));
		    }
		}
		ZeroAll();
		//glUseProgram(0);
	}
	~Surface_class()
	{
		delete[] plane;
	}
	void ZeroAll()
	{
			for(int i = 0; i < width*length; ++i)
			{
					plane[i].pos.pos.y =0.0;
					plane[i].div_h = 0.0;
					plane[i].ddiv_x = 0.0;
					plane[i].ddiv_y = 0.0;
					plane[i].ddiv_h =0.0;
			}
	}
	void calculateSurfaceNormals()
	{
        //zero it all first
        for(int x  = 0; x < width*length; ++x)
        {
            plane[x].normal = vec(0,0,0);
        }
        for(int x = 0; x < width-1; ++x)
		{
			for(int y = 0; y < length-1; ++y)
			{
				  //> cross ^
				addnorm(x,y,((accessVert(x,y).pos - accessVert(x+1,y).pos)%(accessVert(x,y+1).pos - accessVert(x,y).pos)).toUnit());    //> cross ^
			}
		}
        for(int x = 1; x < width; ++x)
		{
			for(int y = 0; y < length-1; ++y)
			{
				//addnorm(x,y,((accessVert(x,y+1).pos - accessVert(x,y).pos)%(accessVert(x-1,y).pos - accessVert(x,y).pos)).toUnit());    //^ cross <
                addnorm(x,y,((accessVert(x,y).pos - accessVert(x,y+1).pos)%(accessVert(x-1,y).pos - accessVert(x,y).pos)).toUnit());    //^ cross <

			}
		}
        for(int x = 1; x < width; ++x)
		{
			for(int y = 1; y < length; ++y)
			{
				//addnorm(x,y,((accessVert(x-1,y).pos - accessVert(x,y).pos)%(accessVert(x,y-1).pos - accessVert(x,y).pos)).toUnit());    //< cross V
				addnorm(x,y,((accessVert(x,y).pos - accessVert(x-1,y).pos)%(accessVert(x,y-1).pos - accessVert(x,y).pos)).toUnit());    //< cross V

			}
		}
		for(int x = 0; x < width-1; ++x)
		{
			for(int y = 1; y < length; ++y)
			{
				//addnorm(x,y,((accessVert(x,y-1).pos - accessVert(x,y).pos)%(accessVert(x+1,y).pos - accessVert(x,y).pos)).toUnit());    //V cross >
				addnorm(x,y,((accessVert(x,y).pos - accessVert(x,y-1).pos)%(accessVert(x+1,y).pos - accessVert(x,y).pos)).toUnit());    //V cross >
			}
		}
	}
	void addnorm(int x, int y, vec input)
	{
	    plane[x+y*width].normal += input;
	}
	void setpos(int x, int y, vec input)
	{
	    plane[x+y*width].pos = input;
	}
	void draw_surface(GLint shaderprogram) //renders new surface
	{
        glUniform3f(glGetUniformLocation(shaderprogram,"samb"),0.1,0.1,0.5);
        glUniform3f(glGetUniformLocation(shaderprogram,"sdif"),0.1,0.1,0.9);
        glUniform3f(glGetUniformLocation(shaderprogram,"sspec"),1.0,1.0,1.0);
        glUniform1f(glGetUniformLocation(shaderprogram,"ref_number"),30.0);
		for(int x = 0; x < width-1; ++x)
		{
			glBegin(GL_TRIANGLE_STRIP);
			for(int y = 0; y < length; ++y)
			{
				//unshown, truncated vertex used for ligthing
				//add gl normal for normal vector and ligthing
				vec posi = accessVert(x,y).pos;
				vec norm = accessVert(x,y).normal;
				glVertex3f(posi.pos.x,posi.pos.y,posi.pos.z);
				glNormal3f(norm.pos.x,norm.pos.y,norm.pos.z);
				posi = accessVert(x+1,y).pos;
				norm = accessVert(x+1,y).normal;
				glVertex3f(posi.pos.x,posi.pos.y,posi.pos.z);
                glNormal3f(norm.pos.x,norm.pos.y,norm.pos.z);
			}
			glEnd();

		}


	}
	vert accessVert(int x, int y)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
			return plane[x+y*width];
		}
		else
		{
			throw printf("acessVert out of bounds error!!!\n");
		}
	}
	void setheights()
	{
		//printf("this is reached\n");
		DDivXsurface();
		DDivYsurface();
		surfaceHeight();
		if(rain)
		{
		    if(rand()%4 == 3)
		    {
		        RainDrop(rand()%width, rand()%length);
		    }
		}
	}
	void DDivXsurface()
	{
			//0 edge
			for(int i = 0; i < length; ++i)
			{
					setDDivx(0,i,(accessVert(2,i).pos.pos.y + accessVert(1,i).pos.pos.y-2*accessVert(0,i).pos.pos.y)/ddist); //dirty hack , possible reflection
			}
			//width edge (it's width -1)
			for(int i = 0; i<length; ++i)
			{
					setDDivx(width-1,i,(accessVert(width-3,i).pos.pos.y + accessVert(width-2,i).pos.pos.y-2*accessVert(width-1,i).pos.pos.y)/(ddist)); //dirty hack , possible reflection
			}
			for(int i = 0; i < length; ++i)
				for(int j = 1; j < width-1; ++j)
				{
					setDDivx(j,i,(accessVert(j-1,i).pos.pos.y + accessVert(j+1,i).pos.pos.y - 2*accessVert(j,i).pos.pos.y)/ddist);
				}
	}
	void DDivYsurface()
	{
			//0 edge
			for(int i = 0; i < width; ++i)
			{
					setDDivy(i,0,(accessVert(i,2).pos.pos.y + accessVert(i,1).pos.pos.y-2*accessVert(i,0).pos.pos.y)/ddist); //dirty hack , possible reflection
			}

			for(int i = 0; i<width; ++i)
			{
					setDDivy(i,length-1,(accessVert(i,length-2).pos.pos.y + accessVert(i,length-3).pos.pos.y-2*accessVert(i,length-1).pos.pos.y )/ddist); //backwards
			}
			for(int i = 0; i < width; ++i)
				for(int j = 1; j < length-1; ++j)
				{
					setDDivy(i,j,(accessVert(i,j-1).pos.pos.y + accessVert(i,j+1).pos.pos.y - 2*accessVert(i,j).pos.pos.y)/ddist);
				}
	}
	void surfaceHeight()
	{
		//printf("i have been reached\n");
			for(int i = 0; i< length*width; ++i)
			{
				plane[i].ddiv_h = plane[i].ddiv_x + plane[i].ddiv_y;
				//if(plane[i].ddiv_h > 15)
					//printf("plane[%d].ddiv_h = %f",i,plane[i].ddiv_h);
				plane[i].div_h += time*plane[i].ddiv_h;
				//if(plane[i].div_h > 15)
					//printf("plane[%d].div_h = %f",i,plane[i].div_h);
				plane[i].pos.pos.y = plane[i].pos.pos.y*DAMP + time*plane[i].div_h;

			}
	}
	void setTime(GLfloat input)
	{
		time = input;
	}
	float dampsin(float input)
	{
		if(input == 0.0)
		{
			return 1.0;
		}
		else
		{
			return sin(input)/input;
		}
	}
	void throwstone(int i, int j)
	{
		setDivh(i,j,-10.0);
	}
	void switchFillMode()
	{
		switch(fill)
		{
			case LINE:
					fill = FILL;
					//glUseProgram(shaderprogram);
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			break;
			case FILL:
					fill = LINE;
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					//glUseProgram(0);
			break;
		}
	}
	void RainDrop(int x, int y)
	{
        setDivh(x,y,accessVert(x,y).div_h -0.5);
	}
	void RainSwitch()
	{
	    rain = !rain;
	}
	void setHeight(int x, int y, GLfloat value)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
				plane[x+y*width].pos.pos.y = value;
		}
	}
	void setDivh(int x, int y, GLfloat value)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
			plane[x+y*width].div_h = value;
		}
	}
	void setDDivx(int x, int y, GLfloat value)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
			plane[x+y*width].ddiv_x = value;
		}
	}
	void setDDivy(int x, int y, GLfloat value)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
			plane[x+y*width].ddiv_y = value;
		}
	}
	void setDDivh(int x, int y, GLfloat value)
	{
		if(x < width && y < length && x >= 0 && y >= 0)
		{
			plane[x+y*width].ddiv_h = value;
		}
	}

    void BallLand(GLfloat x, GLfloat y, GLfloat rad)
    {
        if(x > 0 && y > 0 && y < length*dist && x < width*dist) //calculate widths
        {
            GLfloat xmod = fmod(x,dist);
            GLfloat ymod = fmod(y,dist);
            GLint intx = (GLint)(x/dist);
            GLint inty = (GLint)(y/dist);
            if(xmod > dist/2)
            {
                ++intx;
            }
            if(ymod > dist/2)
            {
                ++inty;
            }
            throwstone(intx,inty);
        }
    }

};
#endif
