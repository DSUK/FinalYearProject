﻿#include "Surface.h"
#include "Vec.h"

#ifndef _MSC_VER
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL.h>
#else
#include <cstdio>
#include <SDL_opengl.h>
#include <SDL.h>
#endif
#include <cmath>

#include "Mat4x4.h"
#include "InputHandler.h"

float range(float min, float num, float max) {
	if(num > max) {
		return max;
	} if(num < min) {
		return min;
	}
	return num;
}
void handle_resize(Uint32 window_id, int x, int y) {
	SDL_Window *window = SDL_GetWindowFromID(window_id);
	glViewport(0,0,x,y);
	SDL_assert(window != nullptr);
	SDL_SetWindowSize(window,x,y);
	GLint Program;
	auto projectionMatrix = Mat4x4::Perspective(kFovy, (GLfloat)x / (GLfloat)y, kPerspectiveNear, kPersperciveFar);
	glGetIntegerv(GL_CURRENT_PROGRAM, &Program);
	glUniformMatrix4fv(glGetUniformLocation(Program,"ProjectionMatrix"),1,GL_FALSE, &projectionMatrix(0,0));
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
						directionvcoe += 0.01f;
					break;
					case SDLK_s:
					case SDLK_DOWN:
						directionvcoe -= 0.01f;
					break;
					case SDLK_a:
					case SDLK_LEFT:
						orthocoe -= 0.005f;
					break;
					case SDLK_d:
					case SDLK_RIGHT:
						orthocoe +=0.005f;
					break;
					case SDLK_r:
						heightmap->rainSwitch();
					break;
					case SDLK_F5:
						printf("rotation = ( %f, %f, %f )\n", rotation.x, rotation.y, rotation.z);
						printf("position = ( %f, %f, %f )\n", position.x, position.y, position.z);
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
						directionvcoe -= 0.01f;
					break;
					case SDLK_s:
					case SDLK_DOWN:
						directionvcoe +=0.01f;
					break;
					case SDLK_a:
					case SDLK_LEFT:
						orthocoe +=0.005f;
					break;
					case SDLK_d:
					case SDLK_RIGHT:
						orthocoe -= 0.005f;
					break;
					default:
					break;
				}
			break;
			case SDL_QUIT:
				return 0;
			break;
			case SDL_WINDOWEVENT:
				if(event.window.event == SDL_WINDOWEVENT_RESIZED) {
					handle_resize(event.window.windowID,event.window.data1,event.window.data2);
				}
			break;
			case SDL_MOUSEBUTTONDOWN:
				if(event.button.button == SDL_BUTTON_LEFT) {
					return 2;
				}
			break;
			case SDL_MOUSEMOTION:
				if(camera) {
					rotation.x += (GLfloat)event.motion.xrel;
					rotation.x = fmod(rotation.x, 360.f);
					rotation.y += (GLfloat)event.motion.yrel;
					rotation.y = range(-90.0,rotation.y,90);
					direction_vector.x = sinf(rotation.x*degtorad)*cosf(
						rotation.y*degtorad);
					direction_vector.y = -sinf(rotation.y*degtorad);
					direction_vector.z = -cosf(rotation.x*degtorad)*cosf(
						rotation.y*degtorad);
					ortho.x = cosf(rotation.x*degtorad);
					ortho.z = sinf(rotation.x*degtorad);
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
	glRotatef(rotation.x,0.0,1.0,0.0);
	glRotatef(rotation.y,cos(rotation.x*degtorad),0.0,sin(rotation.x*degtorad));
	glTranslatef(-position.x,-position.y,-position.z);
}
