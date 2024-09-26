#include "Game.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>

int main()
{
	//srand((unsigned)time(NULL));;
	GameEngine g("bin/assets.txt");
	g.run();
	return 0;
}