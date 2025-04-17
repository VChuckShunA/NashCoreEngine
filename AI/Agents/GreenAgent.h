#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../BehaviourTrees/StatefulSequence.h"
#include "../BehaviourTrees/Loop.h"
#include "../../EntityManager.h"

#include <SFML/System.hpp>

class AIPlayroom;
    enum ItemType { FOOD, WEAPON };
    enum EnemyState { NONE, VISIBLE };
    enum AgentState { IDLE, SEARCHING, FIGHTING, FLEEING, HEALING };
    class GreenAgent
    {
    private:
        
    public:
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom); 
        std::vector<Vec2> currentpath;
        Vec2 Waypoint1 = Vec2(19, 11);
        Vec2 Waypoint2 = Vec2(4, 6);
        Vec2 Waypoint3 = Vec2(0, 11);
        const std::shared_ptr<Entity>& agent;
        AIPlayroom* room;
        Node* BehaviourTree;
        void update();
        bool hasWeapon = true;
        bool hasFood = true;
        int health = 199;
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
            
   
    class IsEnemyVisible : public Node {
    public:
        IsEnemyVisible(GreenAgent& agent);
        virtual Status update() override;
    private:
        GreenAgent& agent;
        Vec2 TargetPosition;
    };

    class EngageCombat : public Node {
public:
    EngageCombat(GreenAgent& agent) : agent(agent) {}
    virtual Status update() override {
        // Execute combat actions (e.g., shoot enemy)
        agent.shootEnemy();
        // Here you might return BH_RUNNING until the enemy is neutralized,
        // then return BH_SUCCESS (or BH_FAILURE if combat was interrupted).
        if (agent.hasWeapon) 
        {
            std::cout << "[Combat] Enemy neutralized or lost.\n";
            return BH_SUCCESS;
        }
        return BH_RUNNING;
    }
private:
    GreenAgent& agent;
};

    class MoveToPoint : public Node
    {
    public:
        MoveToPoint(GreenAgent& agent, Vec2& point) :greenAgent(agent), Waypoint(point) {
            
           // std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
        }
    private:
        GreenAgent& greenAgent;
        Vec2& Waypoint;

        virtual void onInitialize() override {
           
                greenAgent.initializeMoveToPoint(Waypoint);
            
        }
        
        virtual Status update() override {
            
            if (!greenAgent.destinationReached)
            {
                greenAgent.MoveToPoint(Waypoint);
                return BH_RUNNING; //Not reached destination 
            }
            else if (greenAgent.destinationReached) {
                return BH_SUCCESS; // Reached the point = success
            }
               
        }
    };

    //class WaitForSeconds : public Node
    //{
    //public:
    //    WaitForSeconds(GreenAgent& agent, float& Seconds) :greenAgent(agent), waitTime(Seconds) {}
    //private:
    //    GreenAgent& greenAgent;
    //    float& waitTime; float duration = waitTime; // seconds
    //    float timeElapsed = 0;
    //    float time = 60;
    //    virtual void onInitialize() override {
    //        time = 60; // Reset timer at the start of evaluation
    //    }
    //    virtual Status update() override {
    //        timeElapsed += greenAgent.room->clock.getElapsedTime().asSeconds(); // You must track this in your game engine
    //        if (timeElapsed < duration) {
    //            return BH_RUNNING;
    //        }
    //        return BH_SUCCESS;
    //    }

    //};

    class WaitForSeconds : public Node {
    public:
        // duration: number of seconds to wait.
        WaitForSeconds(GreenAgent& agent, float durationSeconds)
            : agent(agent), duration(durationSeconds), elapsed(0.0f)
        {
        }

        // When starting, reset the elapsed time and restart the clock.
        virtual void onInitialize() override {
            elapsed = 0.0f;
            clock.restart();
        }

        virtual Status update() override {
            // Get the elapsed time since the last tick.
            float dt = clock.restart().asSeconds();
            elapsed += dt;
            std::cout << "[WaitForSeconds] Waiting... elapsed: " << elapsed
                << " / " << duration << " seconds" << std::endl;

            // If the elapsed time is less than the duration, still waiting.
            if (elapsed < duration)
                return BH_RUNNING;
            else
                return BH_SUCCESS;
        }

        virtual void reset() override {
            elapsed = 0.0f;
            m_eStatus = BH_INVALID;
        }

    public:
        GreenAgent& agent;
        float duration;   // How many seconds to wait.
        float elapsed;    // Accumulated time.
        sf::Clock clock;  // Clock to measure delta time.
    };

    class Patrol : public StatefulSequence {
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

    class CombatSequence : public Sequence
    {
    public:
        CombatSequence(GreenAgent& agent) {
            addChild(new IsEnemyVisible(agent));
            addChild(new EngageCombat(agent));
        }
    };


    class SurvivalSelector : public Selector {
    public:
        SurvivalSelector(GreenAgent& agent) {
            float time1 = 3;
            float time2 = 3;
            float time3 = 3;
            addChild(new LowHealth(agent));  // First, try healing
            addChild(new CombatSequence(agent)); //If Enemy is in Range, Engage in Combat
          //  addChild(new Patrol(agent));     // patrol the way points
            StatefulSequence* patrolSequence = new StatefulSequence();
            patrolSequence->addChild(new MoveToPoint(agent, agent.Waypoint1));
            patrolSequence->addChild(new WaitForSeconds(agent, time1));
            patrolSequence->addChild(new MoveToPoint(agent, agent.Waypoint2));
            patrolSequence->addChild(new WaitForSeconds(agent, time2));
            patrolSequence->addChild(new MoveToPoint(agent, agent.Waypoint3));
            patrolSequence->addChild(new WaitForSeconds(agent, time3));
            addChild(new Loop(patrolSequence));
        }
    };

