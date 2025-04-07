#include "GreenAgent.h"
#include <iostream>

GreenAgent::GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom) :agent(entity),room(playroom)
{

    BehaviourTree = new SurvivalSelector(*this);
    path1 = room->navmesh.FindPath(room->positionToGridCordinates(agent), Waypoint1);
    currentpath = path1;
}

void GreenAgent::update()
{
    BehaviourTree->tick(); // Runs the tree
   // std::cout << "Tick " << health << std::endl;
}

void GreenAgent::consumeFood()
{
    if (hasFood) {
        health = std::min(maxHealth, health + 50);
        hasFood = false;
        std::cout << "Consumed food. Health: " << health << std::endl;
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

void GreenAgent::patrol(std::vector<Vec2> pathToFollow)
{
    static float patrolCooldown = 0; // Timer for switching paths
    int AISpeed = 64;
    int pathcount = 1;
    bool destinationReached = false;
    bool up = false, down = false, left = false, right = false;
    float angleToWaypoint;
    Vec2 A = Vec2((int)room->positionToGridCordinates(agent).x, (int)room->positionToGridCordinates(agent).y);
    Vec2 B = Vec2(currentpath.front().x, currentpath.front().y);

    //0=right,90=down,180 =left, 270=up
    Vec2 distanceBetween;
    if (!destinationReached)
    {

        if (!currentpath.empty()) {
            distanceBetween = Vec2(abs(agent->getComponent<CTransform>().pos.x - room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x), abs(agent->getComponent<CTransform>().pos.y - room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y));

            if (distanceBetween.x < 5 && distanceBetween.y < 5)
            {
                currentpath.erase(currentpath.begin());
            }
            //TODO: Find a cleaner a way to do this
            if (agent->getComponent<CTransform>().pos.x < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move Left
                agent->getComponent<CTransform>().pos.x = agent->getComponent<CTransform>().pos.x + AISpeed;
                left = false;
                right = true;
            }if (agent->getComponent<CTransform>().pos.x > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move Right
                agent->getComponent<CTransform>().pos.x = agent->getComponent<CTransform>().pos.x - AISpeed;
                left = true;
                right = false;
            }
            if (agent->getComponent<CTransform>().pos.y < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Down
                agent->getComponent<CTransform>().pos.y = agent->getComponent<CTransform>().pos.y + AISpeed;
                down = true;
                up = false;

            }if (agent->getComponent<CTransform>().pos.y > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Up
                agent->getComponent<CTransform>().pos.y = agent->getComponent<CTransform>().pos.y - AISpeed;
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
           currentpath=path2; 
        }
    }
}


