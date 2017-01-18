#include <chrono>
#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#ifndef _MSC_VER
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif

#include "Vec.h"
#include "Surface.h"
#include "InputHandler.h"
#include "ThrowContainer.h"

const int WINDOW_HEIGHT = 768;
const int WINDOW_WIDTH = 1024;

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<GLfloat> fsecond;

void loadShaders(unsigned int*,unsigned int*, unsigned int*);
void DrawBall(GLfloat,Vec pos);
void programLoop(SDL_Window *window);


void programLoop(SDL_Window *window)
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
	ThrowContainer ballList;
	glMatrixMode(GL_MODELVIEW);
    auto loop_end_time = Time::now();
    auto loop_start_time = Time::now();
	while(continueloop)
	{
		continueloop = mouse.handleEvent(&heightmap);
		loop_start_time = loop_end_time;
		loop_end_time = Time::now();
		fsecond delta = (loop_end_time - loop_start_time);
		mouse.movement(delta.count()*256.0f);
		ballList.cullLow();
		ballList.moveobjects(0.05,0.0005,&heightmap);
		switch(continueloop)
		{
			case 2:
				ballList.addtolist(mouse.getDirectionVector(),mouse.getPosition());
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
		ballList.drawobjects(0.5);
		glFlush();
		SDL_GL_SwapWindow(window);

	}
	glDetachShader(program,vertex);
	glDetachShader(program,frag);
	glDeleteShader(vertex);
	glDeleteShader(frag);
	glDeleteProgram(program);

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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
		argv[0],
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	SDL_assert(window != nullptr);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);

	GLenum err = glewInit();

	if(err!= GLEW_OK)
	{
		fprintf(stderr,"Glew error: %s\n",glewGetErrorString(err));
		SDL_assert(err != GLEW_OK);
	}
	programLoop(window);
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
