#include "Ammo.h"
#include <iostream>
Ammo::Ammo(const std::shared_ptr<Entity>& entity) :Item(std::move(entity)) {
	type = ITM_AMMO;
}

void Ammo::AddToPlayer(AIPlayroom* room)
{
	room->ammoCount += 3;
	std::cout << "Added Ammo to Player" << std::endl;
	HandleRemoval(room);
}