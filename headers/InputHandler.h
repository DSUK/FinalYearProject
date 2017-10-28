#pragma once

class Surface;
struct Vec;
#define degtorad (3.141592653589793/180.0)
class InputHandler {

	bool camera;
	Vec position;
	Vec rotation;
	Vec direction_vector;
	Vec ortho;
	SDL_Event event;
	GLfloat directionvcoe;
	GLfloat orthocoe;

	public:
	InputHandler();
	Vec getPosition();
	Vec getDirectionVector();
	int handleEvent(Surface *heightmap);
	void movement(float delta);
	void preRenderTranslate();
};
