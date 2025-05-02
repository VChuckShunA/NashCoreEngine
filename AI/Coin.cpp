#include "Coin.h"
Coin::Coin(const std::shared_ptr<Entity>& entity) :Item(std::move(entity)) {
	type = ITM_COIN;
}

void Coin::AddToPlayer()
{
}

void Coin::UseItem()
{
}
