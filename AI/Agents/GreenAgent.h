#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../../EntityManager.h"
#include "../AIPlayroom.h"

class AIPlayroom;
    enum ItemType { FOOD, WEAPON };
    enum EnemyState { NONE, VISIBLE };
    enum AgentState { IDLE, SEARCHING, FIGHTING, FLEEING, HEALING };
    class GreenAgent
    {
    private:
        
    public:
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom); std::vector<Vec2> currentpath, path1, path2, path3;
        std::vector< std::vector<Vec2>> paths = { path1,path2,path3 };
        Vec2 Waypoint1 = Vec2(19, 11);
        Vec2 Waypoint2 = Vec2(10, 0);
        Vec2 Waypoint3 = Vec2(4, 10);
        const std::shared_ptr<Entity>& agent;
        AIPlayroom* room;
        Node* BehaviourTree;
        void update();
        bool hasWeapon = false;
        bool hasFood = true;
        int health = 1;
        int maxHealth = 100;
        bool houseVisible = false;
        bool itemVisible = false;
        ItemType visibleItemType;
        EnemyState enemyState = NONE;
        void updateCurrentPath(const Vec2& Destination);
        bool destinationReached = false;
        void initializeMoveToPoint(const Vec2& Destination);
        void pickUpItem(ItemType item);
        void consumeFood();
        void shootEnemy();
        void flee();
        void enterHouse();
        void searchHouse();
        void steer(float targetAngle);
        void MoveToPoint(const Vec2& Waypoint);
    };

    class LowHealth : public Node
    {
    public:
        LowHealth(GreenAgent& agent) :greenAgent(agent) {}
    private:
        GreenAgent& greenAgent;
        virtual Status update() override {
            if (greenAgent.health <= 0)
            {
                //dead
                return BH_SUCCESS;
            }
            if (greenAgent.health < 25) {
                if (greenAgent.hasFood) {
                    greenAgent.consumeFood();
                    greenAgent.hasFood = !greenAgent.hasFood;
                    std::cout << "Successfully healed" << std::endl;
                    return BH_SUCCESS; // Successfully healed
                }
                std::cout << "No food, can't heal" << std::endl;
                return BH_FAILURE; // No food, can't heal
            }
            std::cout << "Health is above 25, continue other tasks" << std::endl;
            return BH_FAILURE; // Health is above 25, continue other tasks
        }
    };
            
    //class Patrol : public Node {
    //public:
    //    Patrol(GreenAgent& agent, Vec2& Destination) : greenAgent(agent), Destination(Destination) {
    //        greenAgent.destinationReached = false;
    //        greenAgent.updateCurrentPath(Destination);
    //        std::cout << "Destination Reset to : " << Destination.x << " , " << Destination.y << std::endl;
    //    }

    //    virtual Status update() override {
    //        greenAgent.patrol(greenAgent.currentpath);
    //        std::cout << "Patrolling" << std::endl;
    //        if (greenAgent.destinationReached)
    //        {
    //            std::cout << "Destination Reached" << std::endl;
    //            Destination = greenAgent.Waypoint2;
    //            greenAgent.updateCurrentPath(Destination);
    //            std::cout << "Destination Reset to : "<< Destination.x<<" , "<< Destination.y << std::endl;
    //            greenAgent.health = 1;
    //            return BH_FAILURE; // Health is above 25, continue other tasks
    //        }
    //        return BH_RUNNING; // Keeps running while patrolling
    //    }

    //private:
    //    GreenAgent& greenAgent;
    //    Vec2& Destination;
    //};

   
   
    class MoveToPoint : public Node
    {
    public:
        MoveToPoint(GreenAgent& agent, Vec2& point) :greenAgent(agent), Waypoint(point) {
            greenAgent.initializeMoveToPoint(Waypoint);
            std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
        }
    private:
        GreenAgent& greenAgent;
        Vec2& Waypoint;
        virtual Status update() override {
           
            if (!greenAgent.destinationReached)
            {
                greenAgent.MoveToPoint(Waypoint);
                return BH_RUNNING; //Reached Destination 
            }
            else if (greenAgent.destinationReached)
            {
                return BH_FAILURE; //Reached Destination 
            }
               
        }
    };

    class WaitForSeconds : public Node
    {
    public:
        WaitForSeconds(GreenAgent& agent, float& Seconds) :greenAgent(agent), waitTime(Seconds) {}
    private:
        GreenAgent& greenAgent;
        float& waitTime;
        float time = 60;
        virtual Status update() override {

            std::cout << "Wait For Seconds" << waitTime << std::endl;
            if (time > 0)
            {
                time=time- waitTime;
                std::cout << "Wait For Seconds" << time << std::endl;
                return BH_RUNNING;
            }
            else {

                return BH_SUCCESS; 
            }
        }
    };

    class Patrol : public Sequence {
    public:
        float time1 = 0.3;
        float time2 = 0.5;
        float time3 = 0.7;
        Patrol(GreenAgent& agent) {
            addChild(new MoveToPoint(agent, agent.Waypoint1));    
            addChild(new WaitForSeconds(agent, time1));   
            addChild(new MoveToPoint(agent, agent.Waypoint2));    
            addChild(new WaitForSeconds(agent, time2));   
            addChild(new MoveToPoint(agent, agent.Waypoint3));    
            addChild(new WaitForSeconds(agent, time3));   

        }
    };

    class SurvivalSelector : public Selector {
    public:
        SurvivalSelector(GreenAgent& agent) {
            addChild(new LowHealth(agent));  // First, try healing
            addChild(new Patrol(agent));
            //addChild(new Patrol(agent, agent.Waypoint1));     // If healing fails, patrol
           // addChild(new Patrol(agent, agent.Waypoint2));     // If healing fails, patrol
           // addChild(new Patrol(agent, agent.Waypoint3));     // If healing fails, patrol
        }
    };

