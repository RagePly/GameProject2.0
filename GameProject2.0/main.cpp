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
		//Testing environment
		Sphere* sp = nullptr;
		Sphere sp2(Float3(0, 0, 0), 1, SPHERE_STD);

		if (typeid(sp).hash_code() == typeid(sp2).hash_code())
			std::cout << "They are comparable!" << std::endl;




		


	}
	return 0;
}
