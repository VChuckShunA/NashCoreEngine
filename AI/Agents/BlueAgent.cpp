#include "BlueAgent.h"
#include "../AIPlayroom.h"
#include "../Health.h"
BlueAgent::BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom) //NOTE: std::move is used to transfer ownership
{
	BehaviourTree = std::make_unique<SurvivalSelector>(*this);
	health = 100;
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
	Item* itemPtr = it->get();
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

	for (auto rit = room->inventory.rbegin();
		rit != room->inventory.rend();
		++rit)
	{
		std::cout << "iterating" << std::endl;
		Item* slotPtr = rit->get();     // dereference the reverse_iterator
		if (!slotPtr)             // skip empty slots
		{
			std::cout << "nullptr, skipping" << std::endl;
			continue;
		}

		if (slotPtr->type == Item::ITM_HEALTH)
		{
			std::cout << "Found a health item, returning true" << std::endl;
			return true;
		}
		std::cout << "Item is of type " << slotPtr->type << std::endl;
	}
	return false;

	//for (auto rit = room->inventory.rbegin();
	//	rit != room->inventory.rend();
	//	++rit)
	//{
	//	std::cout << "iterating" << std::endl;
	//	Item* itemPtr = *rit;            // now this is a real pointer from the container
	//	if (itemPtr == nullptr)                    // skip empty slots, if you’re using nullptr for empties

	//	{
	//		std::cout << "nullptr, skipping" << std::endl;
	//		continue;
	//	}
	//	if (itemPtr->type == Item::ITM_HEALTH)
	//	{
	//		std::cout << "Found a health item, returning true" << std::endl;
	//		return true;
	//	}
	//	std::cout << "Item is of type " << itemPtr->type << std::endl;
	//}
	//return false;

	/*return std::any_of(
		room->inventory.begin(),
		room->inventory.end(),
		[](Item* p) {
		return p && p->type == Item::ITM_HEALTH;
	});*/
//	if (room->inventory.empty())
//		return false;
//	
//	 for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
//        //Item* itemPtr = *it;
//        //if (!itemPtr)            // empty slot?
//        //    continue;            // skip it
//
//        if ((*it)->type == Item::ITM_HEALTH) {
//            std::cout << "HAS Food" << std::endl;
//            return true;
//        }
//    }
//    return false;
}

bool BlueAgent::needsAmmo()
{
	std::cout << "derived Implementation needsAmmo" << std::endl;
	int AmmoCount = 0;
	
	if (!room->inventory.empty())
	{
		std::cout << "Needs Ammo: inventory states " << room->inventory.empty() << std::endl;
		for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			Item* itemPtr = it->get();
			if (!itemPtr)            // empty slot?
				continue;            // skip it

			if (itemPtr->type == Item::ITM_AMMO) {
				AmmoCount++;
			}
			std::cout << "Food Count " << AmmoCount << std::endl;
		}
	if (AmmoCount <= 3)
	{
		std::cout << "return true, Ammo Count is " << AmmoCount << std::endl;
		return true;
	}
	else
	{
		std::cout << "return false, Ammo Count is " << AmmoCount << std::endl;
		return false;
	}
	}
	return false;
}

bool BlueAgent::needsFood()
{
	std::cout << "derived Implementation needsFood" << std::endl;

	int FoodCount = 0;
	

	if (!room->inventory.empty())
	{
		std::cout << "Needs Food: inventory states " << room->inventory.empty() << std::endl;

		for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			Item* itemPtr = it->get();
			if (!itemPtr)            // empty slot?
				continue;            // skip it

			if (itemPtr->type == Item::ITM_HEALTH) {
				FoodCount++;
			}
			std::cout << "Food Count " << FoodCount<< std::endl;
			
		}
		if (FoodCount < 3)
		{
			std::cout << "return true, Food Count is " << FoodCount << std::endl;
			return true;
		}
		else
		{
			std::cout << "return false, Food Count is " << FoodCount << std::endl;
			return false;
		}
		//return false;
		/*for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {
			if ((*it)->type == Item::ITM_HEALTH) FoodCount++;
		}
		if (FoodCount <= 3)
			return true;*/
	}
	return false;
}
