#include "Game.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
	//srand((unsigned)time(NULL));
	//Game g("config.txt");
	GameEngine g("bin/config.txt");
	g.run();
	return 0;
}