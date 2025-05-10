#include "BlueAgent.h"

BlueAgent::BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom) //NOTE: std::move is used to transfer ownership
{
	BehaviourTree = std::make_unique<SurvivalSelector>(*this);
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
