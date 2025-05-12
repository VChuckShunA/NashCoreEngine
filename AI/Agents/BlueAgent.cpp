#include "BlueAgent.h"
#include "../AIPlayroom.h"
BlueAgent::BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom) //NOTE: std::move is used to transfer ownership
{
	BehaviourTree = std::make_unique<SurvivalSelector>(*this);
	health = 1;
	//std::cout << "Blue Agent Initialized" << std::endl;
}

void BlueAgent::update()
{
	if (health <= 0) { HandleDeath(); return; }
	BehaviourTree->tick(); // Runs the tree

	//std::cout << "Blue Agent is Running" << std::endl;
}

const int BlueAgent::getHealth()
{
	return health;
}

bool BlueAgent::hasAmmo()
{
	for (auto& it = *room->inventory.rbegin(); it != *room->inventory.rend(); ++it) {
		if (it->type == Item::ITM_AMMO) return true;
	}
	return false;
}

bool BlueAgent::hasFood()
{
	for (auto& it = *room->inventory.rbegin(); it != *room->inventory.rend(); ++it) {
		if (it->type == Item::ITM_HEALTH) return true;
	}
	return false;
}

bool BlueAgent::needsAmmo()
{
	if (room->inventory.size() < 5)
	{
		int AmmoCount = 0;
		for (auto& it = *room->inventory.rbegin(); it != *room->inventory.rend(); ++it) {
			if (it->type == Item::ITM_AMMO) AmmoCount++;
		}
		if (AmmoCount >= 3)
			return false;
	}
	return true;
}

bool BlueAgent::needsFood()
{
	if (room->inventory.size() < 5)
	{
		int FoodCount = 0;
		for (auto& it = *room->inventory.rbegin(); it != *room->inventory.rend(); ++it) {
			if (it->type == Item::ITM_HEALTH) FoodCount++;
		}
		if (FoodCount >= 3)
			return false;
	}
	return true;
}
