#include "GreenAgent.h"
#include <iostream>
#include "../AIPlayroom.h"
#include "../BehaviourTrees/Node.h"




GreenAgent::GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom, Vec2 waypoint1, Vec2 waypoint2, Vec2 waypoint3) : BaseAIAgent(std::move(entity), playroom)
{
   
    BehaviourTree = std::make_unique<PatrolSelector>(*this);
    baseDamageAmount = 25;
    Waypoint1 = waypoint1;
    Waypoint2 = waypoint2;
    Waypoint3 = waypoint3;
}

void GreenAgent::update()
{
    if (health <= 0) { HandleDeath(); return; }
    BehaviourTree->tick(); // Runs the tree
}

void GreenAgent::HandleDeath()
{
    room->PlayerScore+=100;
    BaseAIAgent::HandleDeath();
}

