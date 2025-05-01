#include "BaseAIAgent.h"
#include "../AIPlayroom.h"

BaseAIAgent::BaseAIAgent() : agent(nullptr), room(nullptr) {
}

BaseAIAgent::BaseAIAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) :agent(std::move(entity)), room(playroom)
{
    health = maxHealth;
}

void BaseAIAgent::updateCurrentPath(const Vec2& Destination)
{
  //  std::cout << "GreenAgent 21" << std::endl;

    currentpath = room->navmesh.FindPath(room->positionToGridCordinates(agent), Vec2(Destination.x, Destination.y));
    destinationReached = false;
   // std::cout << "GreenAgent 25" << std::endl;
}

void BaseAIAgent::TakeDamage(int damageAmount)
{
    //NOTE: Due to the iffy collision system, the Damage gets multiplied by 2
    if (health > 0)
    {
        std::cout << "Previous Health " << health << std::endl;
        health -= damageAmount;
        std::cout << "Current Health " << health << std::endl;
    }
    else std::cout << "Player is dead "<< std::endl;
}

void BaseAIAgent::consumeFood()
{
    if (hasFood) {
        health = std::min(maxHealth, health + 50);
        hasFood = false;
        //  std::cout << "Consumed food. Health: " << health << std::endl;
    }
}

void BaseAIAgent::HandleDeath()
{
    BehaviourTree.reset(); 
    BehaviourTree = nullptr;
    room->RemoveAgent(this);
}

void BaseAIAgent::initializeMoveToPoint(const Vec2& Destination)
{ //std::cout << "GreenAgent 28" << std::endl;
   // std::cout << "Destination is: " << Destination.x << " , " << Destination.y << std::endl;
    updateCurrentPath(Destination);
    destinationReached = false;
   // std::cout << "Path Updated: " << Destination.x << " , " << Destination.y << std::endl;
}

void BaseAIAgent::MoveToPoint(const Vec2& Waypoint)
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

void BaseAIAgent::steer(float targetAngle)
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

bool BaseAIAgent::hasTarget()
{
    if (agent->getComponent<CVision>().Target)
        return true;
    return false;
}




IsEnemyVisible::IsEnemyVisible(BaseAIAgent& agent) : agent(agent)
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

EngageCombat::EngageCombat(BaseAIAgent& agent) : agent(agent) {}

Node::Status EngageCombat::update()
{
    // Here you might return BH_RUNNING until the enemy is neutralized,
    // then return BH_SUCCESS (or BH_FAILURE if combat was interrupted).
    if (agent.hasWeapon)
    {
        if(agent.agent->getComponent<CVision>().Target) //NOTE: this had a agent.agent,  it was probably an overseight on my part, so I got rid of it. idk if it broke anything :|
        {
            agent.room->spawnBullet(agent.agent, agent.agent->getComponent<CVision>().Target);
        }
            
        return BH_SUCCESS;
    }
    return BH_RUNNING;
}

TurnTowardsTarget::TurnTowardsTarget(BaseAIAgent& agent, int randomDeviation) :randDev(randomDeviation), agent(agent)
{
}


Node::Status TurnTowardsTarget::update()
{

    agent.room->TurnTowardsTarget(agent.agent, agent.agent->getComponent<CVision>().Target, randDev);
    return BH_SUCCESS;
}
