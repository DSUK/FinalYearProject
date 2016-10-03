#include <list>
#include <chrono>
#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <fstream>
#include <SDL/SDL.h>

#include "Vec.h"
#include "Surface.h"
#include "InputHandler.h"

const int WINDOW_HEIGHT = 768;
const int WINDOW_WIDTH = 1024;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float> fsecond;

void loadShaders(unsigned int*,unsigned int*, unsigned int*);
void DrawBall(GLfloat,Vec);
void programLoop();
struct ball
{
		Vec pos;
		Vec vel;
		bool collide;
		ball(Vec position,Vec velocity)
		{
			pos = position;
			vel = velocity;
			collide = false;
		}
};

class throwcontainer
{
	std::list<ball> objectlist;
	public:
	throwcontainer()
	{
	}
	void drawobjects(GLfloat rad)
	{
			for(std::list<ball>::iterator iter = objectlist.begin(); iter != objectlist.end(); ++iter)
			{
				DrawBall(rad,iter->pos);
			}
	}
	void moveobjects(GLfloat delta_time,GLfloat movement,Surface *surface)
	{
		for(std::list<ball>::iterator iter = objectlist.begin(); iter != objectlist.end(); ++iter)
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
	void cullLow()
	{
		for(std::list<ball>::iterator iter = objectlist.begin(); iter != objectlist.end(); ++iter)
		{
			if(iter->pos.pos.y < -1)
			{
				iter = objectlist.erase(iter);
			}
		}
	}
	void addtolist(Vec vel,Vec pos)
	{
		objectlist.push_back(ball(pos,vel));
	}


};


int main(int argc, char *argv[])
{
	SDL_Surface *surface;
	SDL_Init(SDL_INIT_EVERYTHING);
	surface = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_OPENGL|SDL_RESIZABLE| SDL_DOUBLEBUF);
	if(surface == NULL){ printf("SDL Init Error\n"); return 1;} //error check code
	SDL_WM_SetCaption(argv[0],NULL);
	GLenum err = glewInit();
	if(err!= GLEW_OK)
	{
		std::cerr << "Glew error: " << glewGetErrorString(err) << std::endl;
	}
	programLoop();

	//exit SDL
	SDL_Quit();
	return 0;
}
void programLoop()
{
	unsigned int frag,vertex,program;
	loadShaders(&program,&frag,&vertex);
	int continueloop =1;
	Surface heightmap(100,100,0.05);
	InputHandler mouse;
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0,(GLfloat)WINDOW_WIDTH/(GLfloat)WINDOW_HEIGHT,1.0,700.0);
	glEnable(GL_NORMALIZE);
	throwcontainer balllist;
	glMatrixMode(GL_MODELVIEW);
    auto loop_end_time = Time::now();
    auto loop_start_time = Time::now();
	while(continueloop)
	{
		continueloop = mouse.handleEvent(&heightmap);
		balllist.cullLow();
		balllist.moveobjects(0.05,0.0005,&heightmap);
		loop_start_time = loop_end_time;
		loop_end_time = Time::now();
		fsecond delta = (loop_end_time - loop_start_time);
		mouse.movement(delta.count()*256.0f);
		switch(continueloop)
		{
			case 2:
				balllist.addtolist(mouse.getDirectionVector(),mouse.getPosition());
			break;
			default:
			break;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glLoadIdentity();
		glUniform3f(glGetUniformLocation(program,"lightpos"),-2.0,2.0,-3.0);
        glUniform3f(glGetUniformLocation(program,"amblight"),0.2,0.2,0.2);
		glUniform3f(glGetUniformLocation(program,"diffuselight"),0.8,0.8,0.8);
		glUniform3f(glGetUniformLocation(program,"speclight"),1.0,1.0,1.0);

		mouse.preRenderTranslate();
		heightmap.calculateSurfaceNormals();
		heightmap.setHeights();
		heightmap.drawSurface(program);
		balllist.drawobjects(0.5);
		glFlush();
		SDL_GL_SwapBuffers();

	}
	glDetachShader(program,vertex);
	glDetachShader(program,frag);
	glDeleteShader(vertex);
	glDeleteShader(frag);
	glDeleteProgram(program);

}
void DrawBall(GLfloat rad,Vec pos)
{
	glTranslatef(pos.pos.x,pos.pos.y,pos.pos.z);
	GLUquadricObj* Sphere = gluNewQuadric();
	gluSphere(Sphere,0.1,20,20);
	gluDeleteQuadric(Sphere);
	glTranslatef(-pos.pos.x,-pos.pos.y,-pos.pos.z);
}
void loadShaders(unsigned int *program,unsigned int *FragID,unsigned int *VertID)
{
	*FragID = glCreateShader(GL_FRAGMENT_SHADER);
	std::ifstream input("shaders/fragment.frag");
	if(!input.is_open())
	{
		std::cout << "error loading fragment.frag \n";
		return;
	}
	//concat file into 1 string
	std::string shadersource = "";
	char temp[500];
	while(!input.eof())
	{
		input.getline(temp,500);
		shadersource += temp;
		shadersource += '\n';
	}
	const GLchar* fragsource = shadersource.c_str();
	glShaderSource(*FragID,1,&fragsource,NULL);
	glCompileShader(*FragID);
	char status[1500];
	glGetShaderInfoLog(*FragID,1500,NULL,status);
	std::cout << "Fragment Compile Status : \n" << status << std::endl;



	*VertID = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream input2("shaders/vertex.vert");
	if(!input2.is_open())
	{
		std::cout << "error loading vertex.vert \n";
		return;
	}
	shadersource = "";
	while(!input2.eof())
	{
		input2.getline(temp,500);
		shadersource += temp;
		shadersource += '\n';
	}
	const GLchar* vertsource = shadersource.c_str();
	glShaderSource(*VertID,1,&vertsource,NULL);
	glCompileShader(*VertID);
	glGetShaderInfoLog(*VertID,1500,NULL,status);
	std::cout << "Vertex Compile Status : \n" << status << std::endl;

	*program = glCreateProgram();
	glAttachShader(*program,*VertID);
	glAttachShader(*program, *FragID);
	glLinkProgram(*program);
	glUseProgram(*program);
}