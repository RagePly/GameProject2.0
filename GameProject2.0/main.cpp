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
		//Testing
		World world;

		for (int i = 0; i < 11; i++) {
			Transform tf({ (float) i,i + 1.0f,i + 2.0f }, { i + 3.0f,i + 4.0f });
			Sphere sp((float) i + 5);
			Gobject go(&sp, tf);
			world.add(go);
		}
		
		world.apply(); //fuck this shit

		world.print();


	}
	return 0;
}
