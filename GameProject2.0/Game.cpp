#include "Game.h"


Game::Game(){
	elapsedTime = 0;
	frameRate = 0;
	isRunning = true;
	renderer = NULL;
	window = NULL;
	screen = NULL;
	keys = new cKey[CKEYS_TOTAL];
	counter = 0;
}
Game::~Game(){}

void Game::init(const char* title, int xpos, int ypos, int w, int h, bool fullscreen) {
	std::cout << "Game engine started\nBegining initialization protocol." << std::endl;
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (!SDL_Init(SDL_INIT_EVERYTHING)) { //anything over 0 is wrong, so !0 is true;
		std::cout << "SDL subsystems initialized" << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, w, h, flags);
		if (window) {
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created" << std::endl;
		}

		screen = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, w, h);
		if (screen) {
			std::cout << "The screen was created" << std::endl;
		}

		scrSurf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
		if (scrSurf) {
			std::cout << "Screen Surface Created" << std::endl;
			pitch = scrSurf->pitch;
		}
		
		screenWidth = w;
		screenHeight = h;

		sphere = new Sphere[2];
		
		sphere[0] = Sphere(Float3(0.0f, 1.8 * screenWidth, 0.0f), screenWidth, RGB_COLOR(1,0,0));
		sphere[1] = Sphere(Float3(screenWidth * 0.35, 0.5 * screenWidth, - screenWidth * 0.19), screenWidth * 0.25, RGB_COLOR(0,1,0));
		
		

		sphere[0].print();
		sphere[1].print();
		cam = new Camera(screenWidth / 2, screenWidth, screenHeight, Float2(screenWidth, screenHeight), Float3(0.0f, -500, 0.0f));
		ligth = new Float3(-10 * screenWidth,1.8f * screenWidth, 10.0f * screenWidth);
		
	}
	else {
		isRunning = false;
	}
}


void Game::handleEvents() {
	SDL_Event event;


	SDL_PumpEvents();
	handleKeyboardInput(SDL_GetKeyboardState(NULL));
	updateKeys();

	if (keys[CKEYS_ESC].down) isRunning = false;

	while (SDL_PollEvent(&event) != 0) {
		switch (event.type) 
		{
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
		}
	}

	
}

void Game::update(){
	//this is fucking up my game
	SDL_Delay(17);	


}


void Game::updateKeys() {
	for (int i = 0; i < CKEYS_TOTAL; i++) {
		keys[i].update();
	}
}

void Game::handleKeyboardInput(const Uint8 * keyStates) {
	if (keyStates[SDL_SCANCODE_SPACE]) {
		keys[CKEYS_SPACE].press();
	}

	if (keyStates[SDL_SCANCODE_ESCAPE]) {
		keys[CKEYS_ESC].press();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	
	SDL_LockSurface(scrSurf);
	unsigned char* lockedPixels = (unsigned char*)scrSurf->pixels;

	if (counter < screenHeight) {
		cam->renderRow(sphere, lockedPixels, pitch, counter,*ligth);
		counter++;
	}

	SDL_UpdateTexture(screen, NULL, lockedPixels,scrSurf->pitch);

	SDL_UnlockSurface(scrSurf);


	SDL_RenderCopy(renderer, screen, NULL, NULL);

	SDL_RenderPresent(renderer);
}


void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(screen);
	SDL_FreeSurface(scrSurf);
	SDL_Quit();

	std::cout << "Game cleaned" << std::endl;

}
