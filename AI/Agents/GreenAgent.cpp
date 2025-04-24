#include "GreenAgent.h"
#include <iostream>
#include "../AIPlayroom.h"
#include "../BehaviourTrees/Node.h"




GreenAgent::GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) : BaseAIAgent(std::move(entity), playroom)
{
    BehaviourTree = new SurvivalSelector(*this);
}

void GreenAgent::update()
{
    BehaviourTree->tick(); // Runs the tree
}




void GreenAgent::consumeFood()
{
    if (hasFood) {
        health = std::min(maxHealth, health + 50);
        hasFood = false;
      //  std::cout << "Consumed food. Health: " << health << std::endl;
    }
}





IsEnemyVisible::IsEnemyVisible(GreenAgent& agent): agent(agent)
{
  
}



Node::Status IsEnemyVisible::update()
{

    //agent.enemyState == VISIBLE
    //agent.agent->getComponent<CVision>().seesPlayer
    if (agent.agent->getComponent<CVision>().seesPlayer) {
        return Status::BH_SUCCESS;
    }
    return Status::BH_FAILURE;
}

EngageCombat::EngageCombat(GreenAgent& agent) : agent(agent) {}

Node::Status EngageCombat::update()
{
    // Here you might return BH_RUNNING until the enemy is neutralized,
    // then return BH_SUCCESS (or BH_FAILURE if combat was interrupted).
    if (agent.hasWeapon)
    {
        agent.room->spawnBullet(agent.agent);
        agent.room->TurnTowardsTarget(agent.agent, agent.agent->getComponent<CVision>().Target);
        return BH_SUCCESS;
    }
    return BH_RUNNING;
}
