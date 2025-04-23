#include "GreenAgent.h"
#include <iostream>
#include "../AIPlayroom.h"
#include "../BehaviourTrees/Node.h"



GreenAgent::GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) :agent(entity),room(playroom)
{
    //std::cout << "GreenAgent 6"  << std::endl;
    BehaviourTree = new SurvivalSelector(*this);
    //currentpath = room->navmesh.FindPath(room->positionToGridCordinates(agent), Waypoint1);
   //currentpath = path1;
}

void GreenAgent::update()
{
   // std::cout << "GreenAgent 14" << std::endl;
    BehaviourTree->tick(); // Runs the tree
   // std::cout << "Tick " << health << std::endl;
}

void GreenAgent::updateCurrentPath(const Vec2& Destination)
{
    std::cout << "GreenAgent 21" << std::endl;

    currentpath= room->navmesh.FindPath(room->positionToGridCordinates(agent), Vec2(Destination.x, Destination.y));
    destinationReached = false;
    std::cout << "GreenAgent 25" << std::endl;
   
}

void GreenAgent::initializeMoveToPoint(const Vec2& Destination)
{
    //std::cout << "GreenAgent 28" << std::endl;
    std::cout << "Destination is: " << Destination.x << " , "<< Destination.y << std::endl;
    updateCurrentPath(Destination);
    destinationReached = false;
    std::cout << "Path Updated: " << Destination.x << " , " << Destination.y << std::endl;
}

void GreenAgent::consumeFood()
{
    if (hasFood) {
        health = std::min(maxHealth, health + 50);
        hasFood = false;
      //  std::cout << "Consumed food. Health: " << health << std::endl;
    }
}

void GreenAgent::shootEnemy()
{
    std::cout << "Shooting enemy!" << std::endl;
}

void GreenAgent::steer(float targetAngle)
{
    if (agent->getComponent<CTransform>().angle == targetAngle) return;

    float turnAngle = fmod(targetAngle - agent->getComponent<CTransform>().angle + 360, 360); // Normalize difference

    if (turnAngle < 180)
    {
        //clockwise
        agent->getComponent<CTransform>().angle++;
    }
    if (turnAngle > 180)
    {
        //counter clock wise
        agent->getComponent<CTransform>().angle--;
    }
    agent->getComponent<CTransform>().angle = fmod(agent->getComponent<CTransform>().angle + 360, 360);
}

void GreenAgent::MoveToPoint(const Vec2& Waypoint)
{
    
    int AISpeed = 2;
    Vec2& AgentCTransform = agent->getComponent<CTransform>().pos;
    bool up = false, down = false, left = false, right = false;
    //0=right,90=down,180 =left, 270=up
    Vec2 distanceBetween;
    if (!destinationReached)
    {

      //  std::cout << "GreenAgent 77" << std::endl;
        if (!currentpath.empty()) {
            distanceBetween = Vec2(abs(AgentCTransform.x - room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x), abs(AgentCTransform.y - room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y));

            if (distanceBetween.x < 5 && distanceBetween.y < 5)
            {
                currentpath.erase(currentpath.begin());
            }
            //TODO: Find a cleaner a way to do this
            if (AgentCTransform.x < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move right
               
                AgentCTransform.x = AgentCTransform.x + AISpeed;
                left = false;
                right = true;
            }if (AgentCTransform.x > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move left
              
                AgentCTransform.x = AgentCTransform.x - AISpeed;
                left = true;
                right = false;
            }
            if (AgentCTransform.y < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Down
              
                AgentCTransform.y = AgentCTransform.y + AISpeed;
                down = true;
                up = false;

            }if (AgentCTransform.y > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Up

              
                AgentCTransform.y = AgentCTransform.y - AISpeed;
                up = true;
                down = false;
            }
            //0=right,90=down,180 =left, 270=up

            if (up && left)
            {
                //entity->getComponent<CTransform>().angle = 225;
                steer(225);
            }
            if (up && right)
            {
                //entity->getComponent<CTransform>().angle = 315;
                steer(315);
            }
            if (down && left)
            {
                // entity->getComponent<CTransform>().angle = 135;
                steer(135);
            }
            if (down && right)
            {
                // entity->getComponent<CTransform>().angle = 45;
                steer(45);
            }
            if (up)
            {
                //entity->getComponent<CTransform>().angle = 270;
                steer(270);
            }
            if (down)
            {
                //entity->getComponent<CTransform>().angle = 90;
                steer(90);
            }
            if (left)
            {
                // entity->getComponent<CTransform>().angle = 180;
                steer(180);
            }
            if (right)
            {
                // entity->getComponent<CTransform>().angle = 0;
                steer(0);
            }

        }
        if (currentpath.empty())
        {
       //     std::cout << "GreenAgent 164" << std::endl;
            destinationReached = true;
        }
    }
}

IsEnemyVisible::IsEnemyVisible(GreenAgent& agent): agent(agent)
{
    std::cout << "IsEnemyVisible(GreenAgent& agent): agent(agent)\n";
}

Node::Status IsEnemyVisible::update()
{
    //TargetPosition = agent.room->positionToGridCordinates(agent.agent->getComponent<CVision>().Target);

    std::cout << "[Combat] Enemy detected.\n";

    std::cout << "enemy is at " << TargetPosition.x << " , " << TargetPosition.y << std::endl;
    //agent.enemyState == VISIBLE
    //agent.agent->getComponent<CVision>().seesPlayer
    if (agent.enemyState == VISIBLE) {
        return Status::BH_SUCCESS;
    }
  //  std::cout << "[Combat] Enemy NOT detected.\n";
    return Status::BH_FAILURE;
}

EngageCombat::EngageCombat(GreenAgent& agent) : agent(agent) {}

Node::Status EngageCombat::update()
{
    std::cout << "[EngageCombat] Update.\n";
    // Execute combat actions (e.g., shoot enemy)
    agent.shootEnemy();
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
