#include "GreenAgent.h"
#include <iostream>
#include "../AIPlayroom.h"
#include "../BehaviourTrees/Node.h"




GreenAgent::GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom)
{
    BehaviourTree = std::make_unique<SurvivalSelector>(*this);
}

void GreenAgent::update()
{
    BehaviourTree->tick(); // Runs the tree
}

