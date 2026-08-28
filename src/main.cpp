#define SDL_MAIN_HANDLED 1
#include "game.hpp"
#include <emscripten.h>
#include <iostream>

jadeite::Game game{};

void main_loop()
{
	game.Run();
}

int main()
{	
	if (!game.Initialize())
	{
		std::cerr << "Failed to initialize game.\n";
		return -1;
	}
	
	std::cout << "Starting Game...\n";
	emscripten_set_main_loop(main_loop, 0, 1);
	
	return 0;
}
