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
		Transform pos1({ -1,3,-1 });
		Transform pos2({ 1,3,-1 });
		Transform pos3({ 1,4,-1 });
		Transform pos4({ -1,4,-1 });
		Transform pos5({ -1,3,1 });
		Transform pos6({ 1,3,1 });
		Transform pos7({ 1,4,1 });
		Transform pos8({ -1,4,1 });

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
		RastCam* rastCam = new RastCam(camera, Int2(w, h), Float2(1.0f, 0.5625f), 0.7f);

		rast = new Rasterizer();
		painter = new Painter(w, h);
		player = new Player(camera);

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


	player->update(keys);
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

	if (keyStates[SDL_SCANCODE_W]) {
		keys[CKEYS_W].press();
	}

	if (keyStates[SDL_SCANCODE_A]) {
		keys[CKEYS_A].press();
	}

	if (keyStates[SDL_SCANCODE_S]) {
		keys[CKEYS_S].press();
	}

	if (keyStates[SDL_SCANCODE_D]) {
		keys[CKEYS_D].press();
	}

	if (keyStates[SDL_SCANCODE_R]) {
		keys[CKEYS_R].press();
	}

	if (keyStates[SDL_SCANCODE_F]) {
		keys[CKEYS_F].press();
	}

	if (keyStates[SDL_SCANCODE_UP]) {
		keys[CKEYS_UP].press();
	}

	if (keyStates[SDL_SCANCODE_LEFT]) {
		keys[CKEYS_LEFT].press();
	}

	if (keyStates[SDL_SCANCODE_DOWN]) {
		keys[CKEYS_DOWN].press();
	}

	if (keyStates[SDL_SCANCODE_RIGHT]) {
		keys[CKEYS_RIGHT].press();
	}
}

void Game::render() {
	SDL_RenderClear(renderer);
	
	SDL_LockSurface(scrSurf);
	unsigned char* lockedPixels = (unsigned char*)scrSurf->pixels;

	painter->beginDrawing(lockedPixels);

	rast->renderBackground();

	rast->renderImage();

	rast->drawLine(rast->tfPToScr(world->getAllGobj()[0].tf.pos).pos, rast->tfPToScr(world->getAllGobj()[5].tf.pos).pos);

	painter->finishDrawing();


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

















//Player

Player::Player()
	:
	eyes(nullptr)
{}

Player::Player(Gobject* camera) {
	eyes = camera;
}

Player::~Player() 
{}

void Player::update(cKey* keys) {
	float drotZ(0.0f);
	float drotX(0.0f);

	float dx(0.0f);
	float dy(0.0f);
	float dz(0.0f);

	bool upd = false;

	if (keys[CKEYS_UP].down) {
		drotX = -TURNING_INCREMENT;
		upd = true;
	}
	else if (keys[CKEYS_DOWN].down) {
		drotX = TURNING_INCREMENT;
		upd = true;
	}

	if (keys[CKEYS_RIGHT].down) {
		drotZ = -TURNING_INCREMENT;
		upd = true;
	}
	else if (keys[CKEYS_LEFT].down) {
		drotZ = TURNING_INCREMENT;
		upd = true;
	}

	if (keys[CKEYS_W].down) {
		dy = WALKING_INCREMENT;
		upd = true;
	}
	else if (keys[CKEYS_S].down) {
		dy = -WALKING_INCREMENT;
		upd = true;
	}

	if (keys[CKEYS_D].down) {
		dx = WALKING_INCREMENT;
		upd = true;
	}
	else if (keys[CKEYS_A].down) {
		dx = -WALKING_INCREMENT;
		upd = true;
	}

	if (keys[CKEYS_R].down) {
		dz = WALKING_INCREMENT;
		upd = true;
	}
	else if (keys[CKEYS_F].down) {
		dz = -WALKING_INCREMENT;
		upd = true;
	}


	if (!upd) {
		return;
	}

	eyes->tf.rot.add(Float2(drotX, drotZ));

	float cos1 = cos(eyes->tf.rot.getX());
	float cos2 = cos(eyes->tf.rot.getY());
	float sin1 = sin(eyes->tf.rot.getX());
	float sin2 = sin(eyes->tf.rot.getY());


	fmat3x1 p;
	p.mat[0] = dx;
	p.mat[1] = dy;
	p.mat[2] = dz;

	//rotating around origo and move origo to the eye (which is distToScr neg y direction from camera)

	//rotate x
	/*
		|	1	0	0		|
	Rx	|	0	cos	-sin	|
		|	0	sin	cos		|
	*/

	fmat3x3 Rx;
	Rx.mat[0][0] = 1.0f;
	Rx.mat[1][1] = cos1;
	Rx.mat[1][2] = -sin1;
	Rx.mat[2][1] = sin1;
	Rx.mat[2][2] = cos1;

	/*
		|	cos	-sin	0	|
	Rz	|	sin	cos		0	|
		|	0	0		1	|
	*/

	fmat3x3 Rz;
	Rz.mat[0][0] = cos2;
	Rz.mat[0][1] = -sin2;
	Rz.mat[1][0] = sin2;
	Rz.mat[1][1] = cos2;
	Rz.mat[2][2] = 1.0f;

	fmat3x3 Rzx;
	fmat3x1 pP;
	fmat3x3mult(Rz, Rx, Rzx);
	fmat3x3mul3x1(Rzx, p, pP);


	eyes->tf.pos.add(Float3(pP.mat[0], pP.mat[1], pP.mat[2]));


	eyes->tf.rot.print();
	eyes->tf.pos.print();
}
