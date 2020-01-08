#pragma once
#include <SDL.h>
#include <vector>
#include "Rasterizer.h"

struct cKey {
public:
	bool down = false;
	bool held = false;

	void press() {
		pressed = true;
	}

	void update() {
		if (!pressed) {
			down = false;
			held = false;
		}
		else if (down) {
			held = true;
		}
		else {
			down = true;
		}

		pressed = false;
		
	}
private:
	bool pressed = false;
};

enum cKeys {
	CKEYS_DEFAULT,
	CKEYS_SPACE,
	CKEYS_ESC,
	CKEYS_TOTAL
};


class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int w, int h, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool running() {
		return isRunning;
	}


private:
	void updateKeys();
	void handleKeyboardInput(const Uint8* keyStates);
	
	bool isRunning;
	bool memoryAddressReset;
	SDL_Window* window;
	SDL_Renderer* renderer;
	Uint32 elapsedTime;
	Uint32 frameRate;
	cKey* keys;
	SDL_Texture* screen;
	SDL_Surface* scrSurf;
	int screenWidth;
	int screenHeight;
	int pitch;
	int counter;

	Camera* cam;
	World* world;
	Float3* ligth;
};

