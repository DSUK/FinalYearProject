#ifndef _MOUSE_CLASS
#define _MOUSE_CLASS
#define degtorad M_PI/180.0
#include "SurfaceClass.cpp"
//#include "includes.h"
class Surface_class;
void handleresize(int,int);
char foo;

class Mouse_class
{
	float range(float min, float num, float max)
	{
		if(num > max)
		{
			return max;
		}
		if(num < min)
		{
			return min;
		}
		return num;
	}
	bool camera;
	vec position;
	vec rotation;
	vec directionv;
	vec ortho;
	SDL_Event event;
	GLfloat directionvcoe;
	GLfloat orthocoe;

	public:
	Mouse_class()
	{
		camera = false;
		position= vec(1.0,3.9f,0.0f);
		rotation = vec(154.0,49.0f,0.0);
		directionv = vec(0.0,0.0,-1.0);
	}
	vec getposition()
	{
		return position;
	}
	vec getdirectionv()
	{
		return directionv;
	}
	int handleEvent(Surface_class *heightmap) //returns the vector of rotation
	{

		if(SDL_PollEvent(&event)) //if event has changed
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) //press keys
					{
						case SDLK_BACKSPACE:
							heightmap->ZeroAll();
						break;
						case SDLK_ESCAPE:
							return 0;
						break;
						case SDLK_w:
						case SDLK_UP:
							directionvcoe += 0.01;
						break;
						case SDLK_s:
						case SDLK_DOWN:
							directionvcoe -= 0.01;
						break;
						case SDLK_a:
						case SDLK_LEFT:
							orthocoe -= 0.005;
						break;
						case SDLK_d:
						case SDLK_RIGHT:
							orthocoe +=0.005;
						break;
						case SDLK_r:
							heightmap->RainSwitch();
						break;
						case SDLK_F5:
							std::cout << "rotation = (" << rotation.pos.x << "," <<
								rotation.pos.y << "," << rotation.pos.z << ")"<<
								std::endl;
							std::cout << "position = ( " << position.pos.x << "," <<
								position.pos.y << "," << position.pos.z << ")"<<
								std::endl;
						break;
						case SDLK_SPACE:
							camera = !camera;
							SDL_ShowCursor(!SDL_ShowCursor(-1));
						break;
						case SDLK_f:
							heightmap->switchFillMode();
						break;
						case SDLK_F7:
                            position= vec(1.0,3.9f,0.0f);
                            rotation = vec(154.0,49.0f,0.0);
                            directionv = vec(0.0,0.0,-1.0);
                            directionvcoe = 0.0f;
                            orthocoe = 0.0f;
                        break;
						case SDLK_F1:
                            //asm("int $3");
						break;
						default:
						break;
					}
				break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym) //press keys
					{
						break;
						case SDLK_w:
						case SDLK_UP:
							directionvcoe -= 0.01;
						break;
						case SDLK_s:
						case SDLK_DOWN:
							directionvcoe +=0.01;
						break;
						case SDLK_a:
						case SDLK_LEFT:
							orthocoe +=0.005;
						break;
						case SDLK_d:
						case SDLK_RIGHT:
							orthocoe -= 0.005;
						break;
						default:
						break;
					}
				break;
				case SDL_QUIT:
					return 0;
				break;
				case SDL_VIDEORESIZE:
					handleresize(event.resize.w,event.resize.h);
				break;
				case SDL_MOUSEMOTION:case SDL_MOUSEBUTTONUP: case SDL_MOUSEBUTTONDOWN:
					if(event.type == SDL_MOUSEBUTTONDOWN)
					{
						if(event.button.button == SDL_BUTTON_LEFT)
						{
							return 2;
						}
					}
					if(camera && event.type == SDL_MOUSEMOTION)
					{
						rotation.pos.x += (GLfloat)event.motion.xrel;
						rotation.pos.x = fmod(rotation.pos.x, 360);
						rotation.pos.y += (GLfloat)event.motion.yrel;
						rotation.pos.y = range(-90.0,rotation.pos.y,90);
						directionv.pos.x = sin(rotation.pos.x*degtorad)*cos(
							rotation.pos.y*degtorad);
						directionv.pos.y = -sin(rotation.pos.y*degtorad);
						directionv.pos.z = -cos(rotation.pos.x*degtorad)*cos(
							rotation.pos.y*degtorad);
						ortho.pos.x = cos(rotation.pos.x*degtorad);
						ortho.pos.z = sin(rotation.pos.x*degtorad);
					}
				break;
			}
		}
		return 1;
	}
	void movement()
	{
		position = position + (directionvcoe*directionv) + (orthocoe*ortho);
	}
	void PreRenderTranslate()
	{
		glRotatef(rotation.pos.x,0.0,1.0,0.0);
		glRotatef(rotation.pos.y,cos(rotation.pos.x*degtorad),0.0,sin(rotation.pos.x*degtorad));
		glTranslatef(-position.pos.x,-position.pos.y,-position.pos.z);
	}

};
void handleresize(int x, int y)
{
	SDL_SetVideoMode(x,y, 32, SDL_OPENGL|SDL_RESIZABLE| SDL_DOUBLEBUF);
	glViewport(0,0,x,y);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,(GLfloat)x/(GLfloat)y,1.0,700.0);
	glMatrixMode(GL_MODELVIEW);
}
#endif
