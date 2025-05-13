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
{if (room->inventory.empty()) 
        return false;
for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
	Item* itemPtr = *it;
	if (!itemPtr)            // empty slot?
		continue;            // skip it

	if (itemPtr->type == Item::ITM_AMMO) {
		std::cout << "HAS Food" << std::endl;
		return true;
	}
}
return false;
}

bool BlueAgent::hasFood()
{
	return std::any_of(
		room->inventory.begin(),
		room->inventory.end(),
		[](Item* p) {
		return p && p->type == Item::ITM_HEALTH;
	});
	//if (room->inventory.empty())
	//	return false;
	//
	// for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
 //       Item* itemPtr = *it;
 //       if (!itemPtr)            // empty slot?
 //           continue;            // skip it

 //       if (itemPtr->type == Item::ITM_HEALTH) {
 //           std::cout << "HAS Food" << std::endl;
 //           return true;
 //       }
 //   }
 //   return false;
}

bool BlueAgent::needsAmmo()
{
	std::cout << "derived Implementation needsAmmo" << std::endl;
	if (room->inventory.size() < 5)
	{
		int AmmoCount = 0;
		for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			if ((*it)->type == Item::ITM_AMMO) AmmoCount++;
		}
		if (AmmoCount <= 3)
			return true;
	}
	return false;
}

bool BlueAgent::needsFood()
{
	std::cout << "derived Implementation needsFood" << std::endl;


	if (!room->inventory.empty())
	{
		int FoodCount = 0;


		for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			Item* itemPtr = *it;
			if (!itemPtr)            // empty slot?
				continue;            // skip it

			if (itemPtr->type == Item::ITM_HEALTH) {
				FoodCount++;
			}

			if (FoodCount <= 3)
				return true;
		}

		return false;
		/*for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			if ((*it)->type == Item::ITM_HEALTH) FoodCount++;
		}
		if (FoodCount <= 3)
			return true;*/
	}
	return false;
}
