#define SDL_MAIN_HANDLED 1
#include "game.hpp"
#include <emscripten.h>
#include <iostream>

jadeite::Game game{};

/* Loop required for empscripten */
void main_loop()
{
	game.Run();
}

/* Program Entry */
int main()
{	
	emscripten_set_main_loop(main_loop, 0, 1);
	
	return 0;
}
