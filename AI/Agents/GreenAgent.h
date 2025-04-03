#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../../EntityManager.h"
#include "../AIPlayroom.h"

class AIPlayroom;
    enum ItemType { FOOD, WEAPON };
    enum EnemyState { NONE, VISIBLE };
    enum AgentState { IDLE, SEARCHING, FIGHTING, FLEEING, HEALING };
    class GreenAgent
    {
    private:
        std::vector<Vec2> currentpath,path1, path2, path3;
    public:
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom);
        Vec2 Waypoint1 = Vec2(19, 11);
        Vec2 Waypoint2 = Vec2(10, 0);
        Vec2 Waypoint3 = Vec2(0, 11);
        const std::shared_ptr<Entity>& agent;
        AIPlayroom* room;
        Node* BehaviourTree;
        void update();
        bool hasWeapon = false;
        bool hasFood = true;
        int health = 100;
        int maxHealth = 100;
        bool houseVisible = false;
        bool itemVisible = false;
        ItemType visibleItemType;
        EnemyState enemyState = NONE;
     
        void moveTo(const std::string& target);
        void pickUpItem(ItemType item);
        void consumeFood();
        void shootEnemy();
        void flee();
        void enterHouse();
        void searchHouse();
        void steer(float targetAngle);
        void patrol();
    };

    class LowHealth : public Node
    {
    public:
        LowHealth(GreenAgent& agent) :greenAgent(agent) {}
    private:
        GreenAgent& greenAgent;
        virtual Status update() override {
            if (greenAgent.health < 25) {
                if (greenAgent.hasFood) {
                    greenAgent.consumeFood();
                    return BH_SUCCESS; // Successfully healed
                }
                return BH_FAILURE; // No food, can't heal
            }
            return BH_FAILURE; // Health is above 25, continue other tasks
        }
    };
            
    class Patrol : public Node {
    public:
        Patrol(GreenAgent& agent) : greenAgent(agent) {}

        virtual Status update() override {
            greenAgent.patrol();
            return BH_RUNNING; // Keeps running while patrolling
        }

    private:
        GreenAgent& greenAgent;
    };

    class SurvivalSelector : public Selector {
    public:
        SurvivalSelector(GreenAgent& agent) {
            addChild(new LowHealth(agent));  // First, try healing
            addChild(new Patrol(agent));     // If healing fails, patrol
        }
    };


