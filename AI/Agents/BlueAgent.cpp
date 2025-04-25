#include "BlueAgent.h"

BlueAgent::BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom)
{
	//BehaviourTree = std::make_unique<SurvivalSelector>(*this);
	//std::cout << "Blue Agent Initialized" << std::endl;
}

void BlueAgent::update()
{
	//BehaviourTree->tick(); // Runs the tree

	//std::cout << "Blue Agent is Running" << std::endl;
}
