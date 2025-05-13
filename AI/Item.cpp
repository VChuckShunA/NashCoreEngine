#include "Item.h"
#include <iostream>
Item::Item(const std::shared_ptr<Entity>& entity):entity(entity)
{
	type = ITM_NONE;
}

void Item::AddToPlayer(AIPlayroom* room)
{
	std::cout << "Base: Add to player" << std::endl;
}

void Item::HandleRemoval(AIPlayroom* room)
{
	entity->destroy();
	room->RemoveItem(this);
}
