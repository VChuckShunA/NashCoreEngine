#include "Item.h"
#include <iostream>
Item::Item(const std::shared_ptr<Entity>& entity):entity(std::move(entity))
{
	type = ITM_NONE;
}

void Item::AddToPlayer(AIPlayroom* room)
{
	std::cout << "Base: Add to player" << std::endl;
}

void Item::HandleRemoval(AIPlayroom* room)
{
	pickedUp = true;
	entity->destroy();
	room->RemoveItem(this);
}