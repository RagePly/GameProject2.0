//Using SDL and standard IO
#include "Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = WIDTH;
const int SCREEN_HEIGHT = HEIGHT;



int main(int argc, char* args[])
{
	

	if (!TESTING) {
		Game* game = new Game();

		game->init("Game engine test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN);

		while (game->running()) {
			game->handleEvents();
			game->update();
			game->render();
		}

		game->clean();
	}
	else {
	

		


	}
	return 0;
}
