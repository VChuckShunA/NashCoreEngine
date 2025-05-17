#include "Item.h"
#include <iostream>
Item::Item(const std::shared_ptr<Entity>& entity):entity(std::move(entity))
{
	type = ITM_NONE;
}

void Item::HandleRemoval(AIPlayroom* room)
{
	pickedUp = true;
	entity->destroy();
	room->RemoveItem(this);
}