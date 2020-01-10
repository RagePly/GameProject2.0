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
	memoryAddressReset = false;
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

		world = new World();
		Transform pos1({ 0,3,0 });
		Transform pos2({ 1,3,0 });
		Transform pos3({ 1,4,0 });
		Transform pos4({ 0,4,0 });
		Transform pos5({ 0,3,1 });
		Transform pos6({ 1,3,1 });
		Transform pos7({ 1,4,1 });
		Transform pos8({ 0,4,1 });

		world->add(Gobject(pos1));
		world->add(Gobject(pos2));
		world->add(Gobject(pos3));
		world->add(Gobject(pos4));
		world->add(Gobject(pos5));
		world->add(Gobject(pos6));
		world->add(Gobject(pos7));
		world->add(Gobject(pos8));

		Transform camPos({ 0,0,0 });
		Gobject* camera = new Gobject(new CameraObj(), camPos);
		RastCam* rastCam = new RastCam(camera, Int2(w, h), Float2(1.0f, 0.5625f), 1);

		rast = new Rasterizer();
		painter = new Painter(w, h);

		rast->addCamera(rastCam);
		rast->addGameWorldReference(world);
		rast->addPainter(painter);
	}

	else {
		isRunning = false;
	}
}


void Game::handleEvents() {
	SDL_Event event;


	SDL_PumpEvents(); //this overrides the memory position of my shape

	/*
	I therefore have to call this stupid fucking function to reset the memory address.
	
	*/

	if (!memoryAddressReset) {
		world->apply();
		memoryAddressReset = true;
	}

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
	//SDL_Delay(17);	
	
	rast->updateCamStats();

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
	painter->beginDrawing(lockedPixels);

	rast->renderImage();

	painter->finishDrawing();
	//std::cout << "image rendered" << std::endl;


	/*
	if (counter < screenHeight) {
		cam->renderRow(world, lockedPixels, pitch, counter,*ligth);
		counter++;
	}
	*/

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

	delete world;
	delete painter;
	delete rast;

	std::cout << "Game cleaned" << std::endl;

}
