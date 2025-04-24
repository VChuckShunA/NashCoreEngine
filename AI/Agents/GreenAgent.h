#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../BehaviourTrees/StatefulSequence.h"
#include "../BehaviourTrees/Loop.h"
#include "../../EntityManager.h"
#include "BaseAIAgent.h"
#include <SFML/System.hpp>


  
    class GreenAgent : public BaseAIAgent
    {
        
    public:
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom); 
       
        Node* BehaviourTree;
        void update();
        bool hasWeapon = true;
        bool hasFood = false;
        int health = 100;
        int maxHealth = 100;
        bool houseVisible = false;
        bool itemVisible = false;
        void updateCurrentPath(const Vec2& Destination);
        bool destinationReached = false;
        void initializeMoveToPoint(const Vec2& Destination);

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
    EngageCombat(GreenAgent& agent);
    virtual Status update() override;
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

    class CombatSequence : public StatefulSequence
    {
    public:
        CombatSequence(GreenAgent& agent) {
            addChild(new IsEnemyVisible(agent));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 0.5));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 0.5));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 0.5));
        }
    };


    class SurvivalSelector : public Selector {
    public:
        SurvivalSelector(GreenAgent& agent) {
            addChild(new LowHealth(agent));  // First, try healing
            addChild(new CombatSequence(agent)); //If Enemy is in Range, Engage in Combat
            addChild(new Patrol(agent)); //Patrol
        }
    };

