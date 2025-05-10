#include "Health.h"
#include <iostream>
Health::Health(const std::shared_ptr<Entity>& entity) :Item(std::move(entity))
{
	type = ITM_HEALTH;
}

void Health::AddToPlayer(AIPlayroom* room)
{
	std::cout << "Added Health to Player" << std::endl;
	HandleRemoval(room);
}

