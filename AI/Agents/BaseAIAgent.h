#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../BehaviourTrees/StatefulSequence.h"
#include "../BehaviourTrees/Loop.h"
#include "../../EntityManager.h"
#include <SFML/System.hpp>

class AIPlayroom;

class BaseAIAgent
{
private:
public:
	BaseAIAgent();//Default Concstructor
	BaseAIAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom);
	enum ItemType { FOOD, WEAPON };
	enum EnemyState { NONE, VISIBLE };
	enum AgentState { IDLE, SEARCHING, FIGHTING, FLEEING, HEALING };
	ItemType visibleItemType;
	EnemyState enemyState = EnemyState::NONE;
	AIPlayroom* room;
	std::shared_ptr<Entity> agent;
	std::vector<Vec2> currentpath;
	Vec2 Waypoint1 = Vec2(19, 11);
	Vec2 Waypoint2 = Vec2(4, 6);
	Vec2 Waypoint3 = Vec2(0, 11);
	
protected:
	void pickUpItem(ItemType item);

};

