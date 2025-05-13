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

class BaseAIAgent
{
private:
public:
	BaseAIAgent();//Default Concstructor
	std::unique_ptr<Node> BehaviourTree;
	//bool hasAmmo = true;
	//bool hasFood = false;
	int maxHealth = 100;
    int baseDamageAmount = 1;
    int health; //NOTE : Need to clean and build for this to get updated
	bool houseVisible = false;
	bool itemVisible = false;
	bool destinationReached = false;
	BaseAIAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom);

	AIPlayroom* room;
	std::shared_ptr<Entity> agent;
	std::vector<Vec2> currentpath;
	Vec2 Waypoint1 = Vec2(19, 11);
	Vec2 Waypoint2 = Vec2(4, 6);
	Vec2 Waypoint3 = Vec2(0, 11);
    Vec2 ItemPosition;
	virtual void update() = 0;
	void updateCurrentPath(const Vec2& Destination);

    void TakeDamage(std::shared_ptr<Entity> Instigator);
	void shootEnemy();
	void flee();
	void enterHouse();
	void searchHouse();
	void consumeFood();
    void UpdateItemPosition();
    virtual void HandleDeath();
	void initializeMoveToPoint(const Vec2& Destination);
	void MoveToPoint(const Vec2& Waypoint);
	void steer(float targetAngle);
    bool hasTarget();
    virtual bool hasAmmo();
    virtual bool hasFood();
    virtual bool needsFood();
    virtual bool needsAmmo();
protected:


};


class LowHealth : public Node
{
public:
    LowHealth(BaseAIAgent& agent) :greenAgent(agent) { Name = "Low Health Check"; }
private:
	BaseAIAgent& greenAgent;
	virtual Status update() override {
		if (greenAgent.health <= 0)
		{
			//dead
			return BH_SUCCESS;
		}
		if (greenAgent.health < 25) {
			if (greenAgent.hasFood()) {
				greenAgent.consumeFood();
				//greenAgent.hasFood = !greenAgent.hasFood;
				std::cout << "Successfully healed" << std::endl;
				return BH_SUCCESS; // Successfully healed
			}
			std::cout << "No food, can't heal" << std::endl;
			return BH_FAILURE; // No food, can't heal
		}
	//	std::cout << "Health is above 25, continue other tasks" << std::endl;
		return BH_FAILURE; // Health is above 25, continue other tasks
	}
};


class IsEnemyVisible : public Node {
public:
    IsEnemyVisible(BaseAIAgent& agent);
    virtual Status update() override;
private:
    BaseAIAgent& agent;
    Vec2 TargetPosition;
};

class EngageCombat : public Node {
public:
    EngageCombat(BaseAIAgent& agent);
    virtual Status update() override;
private:
    BaseAIAgent& agent;
};

class MoveToPoint : public Node
{
public:
    MoveToPoint(BaseAIAgent& agent, Vec2& point) :greenAgent(agent), Waypoint(point) {
        Name = "Move To Point";
        // std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
    }
private:
    BaseAIAgent& greenAgent;
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


class TurnTowardsTarget : public Node {
private:
    int randDev;
    BaseAIAgent& agent;
public:
    TurnTowardsTarget(BaseAIAgent& agent, int randomDeviation);
    virtual Status update() override;
};

class WaitForSeconds : public Node {
public:
    // duration: number of seconds to wait.
    WaitForSeconds(BaseAIAgent& agent, float durationSeconds)
        : agent(agent), duration(durationSeconds), elapsed(0.0f)
    {
        Name = "Wait For Seconds";
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
       // std::cout << "[WaitForSeconds] Waiting... elapsed: " << elapsed
      //      << " / " << duration << " seconds" << std::endl;

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
    BaseAIAgent& agent;
    float duration;   // How many seconds to wait.
    float elapsed;    // Accumulated time.
    sf::Clock clock;  // Clock to measure delta time.
};

class Patrol : public StatefulSequence {
public:
    float time1 = 0.3;
    float time2 = 0.5;
    float time3 = 0.7;
    Patrol() { Name = "Patrol"; }
    Patrol(BaseAIAgent& agent) {
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
    CombatSequence() { Name = "Combat Sequence"; }
    CombatSequence(BaseAIAgent& agent) {
        addChild(new IsEnemyVisible(agent));
            addChild(new TurnTowardsTarget(agent,12));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 0.5));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 0.7));
            addChild(new EngageCombat(agent));
            addChild(new WaitForSeconds(agent, 2));
    }
};


class CheckSeesFood : public Node {
    BaseAIAgent& ag;
public:
    CheckSeesFood(BaseAIAgent& a) :ag(a) { Name = "Check Sees Food"; }
    Status update() override {
        return ag.agent->getComponent<CVision>().seesFood
            ? BH_SUCCESS
            : BH_FAILURE;
    }
};

class CheckNeedsFood : public Node {
    BaseAIAgent& ag;
public:
    CheckNeedsFood(BaseAIAgent& a) :ag(a) { Name = "Check Needs Food"; }

    virtual void onInitialize() override {

        ag.UpdateItemPosition();

    }
    Status update() override {
       // ag.UpdateItemPosition();
        //&& ag.health < ag.maxHealth
        //Implement needs foods
        return (ag.needsFood())
            ? BH_SUCCESS
            : BH_FAILURE;
    }
};

class CheckSeesAmmo : public Node {
    BaseAIAgent& ag;
public:
    CheckSeesAmmo(BaseAIAgent& a) :ag(a) { Name = "Check Sees Ammo"; }
    Status update() override {
        return ag.agent->getComponent<CVision>().seesAmmo
            ? BH_SUCCESS
            : BH_FAILURE;
    }
};

class CheckNeedsAmmo : public Node {
    BaseAIAgent& ag;
public:
    virtual void onInitialize() override {

        ag.UpdateItemPosition();

    }


    CheckNeedsAmmo(BaseAIAgent& a) :ag(a) { Name = "Check Needs Ammo "; }
    Status update() override {
       // ag.UpdateItemPosition();
        //&& ag.health < ag.maxHealth
       // bool needsAmmo = true;
        //ag.hasAmmo() 
        return (ag.needsAmmo())
            ? BH_SUCCESS
            : BH_FAILURE;
    }
};

class CheckSeesCoin : public Node {
    BaseAIAgent& ag;
public:
    CheckSeesCoin(BaseAIAgent& a) :ag(a) { Name = "Check Sees Coin"; }

    virtual void onInitialize() override {

        ag.UpdateItemPosition();

    }
    Status update() override {
        //ag.UpdateItemPosition();
        return ag.agent->getComponent<CVision>().seesCoin
            ? BH_SUCCESS
            : BH_FAILURE;
    }
};

class ItemFetchSelector : public Selector {
public:
    ItemFetchSelector(BaseAIAgent& ag) {
        Name = "FetchItems";
        
        // Food branch
        Sequence* fetchFood = new Sequence();
        fetchFood->addChild(new CheckSeesFood(ag));
        fetchFood->addChild(new CheckNeedsFood(ag));
        fetchFood->addChild(new MoveToPoint(ag, ag.ItemPosition));
      

        // Ammo branch
        Sequence* fetchAmmo = new Sequence();
        fetchAmmo->addChild(new CheckSeesAmmo(ag));
        fetchAmmo->addChild(new CheckNeedsAmmo(ag));
        fetchAmmo->addChild(new MoveToPoint(ag, ag.ItemPosition));

        // Coin branch (always pick up if seen)
        Sequence* fetchCoin = new Sequence();
        fetchCoin->addChild(new CheckSeesCoin(ag));
        fetchCoin->addChild(new MoveToPoint(ag, ag.ItemPosition));
       

        addChild(fetchFood);
        addChild(fetchAmmo);
        addChild(fetchCoin);
    }
};



class PatrolSelector : public Selector {
public:
    PatrolSelector(){ Name = "Patrol Selector"; }
    PatrolSelector(BaseAIAgent& agent) {
        addChild(new LowHealth(agent));  // First, try healing
        addChild(new CombatSequence(agent)); //If Enemy is in Range, Engage in Combat
        addChild(new Patrol(agent)); //Patrol
    }
};
class SurvivalSelector : public Selector {
public:
    SurvivalSelector(){ Name = "Survival Selector"; }
    SurvivalSelector(BaseAIAgent& agent) {
        addChild(new LowHealth(agent));  // First, try healing
        addChild(new CombatSequence(agent)); //If Enemy is in Range, Engage in Combat
        addChild(new ItemFetchSelector(agent)); //Check for items
        addChild(new Patrol(agent)); //Patrol
    }
};
