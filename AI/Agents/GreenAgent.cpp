#include "GreenAgent.h"

ai::GreenAgent::GreenAgent()
{

    BehaviourTree = new SurvivalSelector(*this);
    
    
}

void ai::GreenAgent::update()
{
    BehaviourTree->tick(); // Runs the tree
    std::cout << "Tick " << health << std::endl;
}

void ai::GreenAgent::consumeFood()
{
    if (hasFood) {
        health = std::min(maxHealth, health + 50);
        hasFood = false;
        std::cout << "Consumed food. Health: " << health << std::endl;
    }
}

void ai::GreenAgent::shootEnemy()
{
    std::cout << "Shooting enemy!" << std::endl;
}

void ai::GreenAgent::patrol()
{
    std::cout << "Patrolling the environment." << std::endl;
}


