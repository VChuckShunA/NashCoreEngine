#include "Health.h"
Health::Health(const std::shared_ptr<Entity>& entity) :Item(std::move(entity))
{
	type = ITM_HEALTH;
}

void Health::AddToPlayer()
{
}

void Health::UseItem()
{
}
