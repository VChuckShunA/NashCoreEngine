#include "BlueAgent.h"

BlueAgent::BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom)
{
	BehaviourTree = std::make_unique<SurvivalSelector>(*this);
}

void BlueAgent::update()
{
	BehaviourTree->tick(); // Runs the tree
}
