#include "InputHandler.h"

float range(float min, float num, float max) {
	if(num > max) {
		return max;
	} if(num < min) {
		return min;
	}
	return num;
}
void handle_resize(int x, int y) {
	SDL_SetVideoMode(x,y, 32, SDL_OPENGL|SDL_RESIZABLE| SDL_DOUBLEBUF);
	glViewport(0,0,x,y);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0,(GLfloat)x/(GLfloat)y,1.0,700.0);
	glMatrixMode(GL_MODELVIEW);
}

InputHandler::InputHandler() {
	camera = false;
	position = Vec(1.0f,3.9f,0.0f);
	rotation = Vec(154.0f,49.0f,0.0f);
	direction_vector = Vec(0.0f,0.0f,-1.0f);
	ortho = Vec(0.0f, 0.0f, 0.0f);
	directionvcoe = 0.0f;
	orthocoe = 0.0f;
}
Vec InputHandler::getPosition() {
	return position;
}
Vec InputHandler::getDirectionVector() {
	return direction_vector;
}
int InputHandler::handleEvent(Surface *heightmap) { //returns an integer signal
	if(SDL_PollEvent(&event)) { //if event has changed
		switch(event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_BACKSPACE:
						heightmap->zeroAll();
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
						heightmap->rainSwitch();
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
						position = Vec(1.0,3.9f,0.0f);
						rotation = Vec(154.0,49.0f,0.0);
						direction_vector = Vec(0.0,0.0,-1.0);
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
				switch(event.key.keysym.sym) {
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
				handle_resize(event.resize.w,event.resize.h);
			break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT) {
					return 2;
				}
			break;
			case SDL_MOUSEMOTION:
				if(camera) {
					rotation.pos.x += (GLfloat)event.motion.xrel;
					rotation.pos.x = fmod(rotation.pos.x, 360);
					rotation.pos.y += (GLfloat)event.motion.yrel;
					rotation.pos.y = range(-90.0,rotation.pos.y,90);
					direction_vector.pos.x = sin(rotation.pos.x*degtorad)*cos(
						rotation.pos.y*degtorad);
					direction_vector.pos.y = -sin(rotation.pos.y*degtorad);
					direction_vector.pos.z = -cos(rotation.pos.x*degtorad)*cos(
						rotation.pos.y*degtorad);
					ortho.pos.x = cos(rotation.pos.x*degtorad);
					ortho.pos.z = sin(rotation.pos.x*degtorad);
				}
			break;
		}
	}
	return 1;
}
void InputHandler::movement(float delta) {
	position = position + delta*((directionvcoe*direction_vector) + (orthocoe*ortho));
}
void InputHandler::preRenderTranslate() {
	glRotatef(rotation.pos.x,0.0,1.0,0.0);
	glRotatef(rotation.pos.y,cos(rotation.pos.x*degtorad),0.0,sin(rotation.pos.x*degtorad));
	glTranslatef(-position.pos.x,-position.pos.y,-position.pos.z);
}
