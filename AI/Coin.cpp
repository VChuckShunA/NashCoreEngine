#include "Coin.h"
#include <iostream>
Coin::Coin(const std::shared_ptr<Entity>& entity) :Item(std::move(entity)) {
	type = ITM_COIN;
}

void Coin::AddToPlayer(AIPlayroom* room)
{
	if(!pickedUp)
		room->PlayerScore = room->PlayerScore+ 10;
	//std::cout << "Added Coin to Player" << std::endl;
	pickedUp = true;
	entity->destroy();
	HandleRemoval(room);
}
