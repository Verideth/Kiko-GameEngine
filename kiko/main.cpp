#include <iostream>
#include "game_manager.h"
#include "cam.h"

GameManager* game_manager = new GameManager(true);

int main(int argc, char* argv[])
{
	game_manager->init();
	game_manager->render();

	std::printf("WARNING: MAIN ABORTED\n");

	delete game_manager;
	
	return 0;
}