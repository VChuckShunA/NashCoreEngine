#pragma once
#include <vector>
#include <iostream>
#include <numbers>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../BehaviourTrees/StatefulSequence.h"
#include "../../EntityManager.h"
#include "../../Physics.h"
#include <SFML/System.hpp>

constexpr float DEG2RAD = std::numbers::pi_v<float> / 180.0f;

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
    bool hasSeenFood = false;
    bool hasSeenAmmo = false;
    bool hasSeenCoin = false;
	bool destinationReached = false;
    Vec2 wallNormal;
	BaseAIAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom);

	AIPlayroom* room;
	std::shared_ptr<Entity> agent;
	std::vector<Vec2> currentpath;
	Vec2 Waypoint1 = Vec2(19, 11);
	Vec2 Waypoint2 = Vec2(4, 6);
	Vec2 Waypoint3 = Vec2(0, 11);
    Vec2 FoodPosition;
    Vec2 CoinPosition;
    Vec2 AmmoPosition;
	virtual void update() = 0;
	void updateCurrentPath(const Vec2& Destination);

    void TakeDamage(std::shared_ptr<Entity> Instigator);
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

class HasAmmo : public Node {
public:
    HasAmmo(BaseAIAgent& agent) :agent(agent)
    {
        Name = "Has Ammo?";
    }
    virtual Status update() override
    {
        if (agent.hasAmmo()) {
            return Status::BH_SUCCESS;
        }
        return Status::BH_FAILURE;
    }
private:
    BaseAIAgent& agent;
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
       // std::cout << "Moving to " << Waypoint.x<< " , " << Waypoint.y << std::endl;
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

class FleeToSafePosition : public Node {
public:
    BaseAIAgent& agent;
    Vec2 oppositeDirection;
    Vec2 safeSpot;
    FleeToSafePosition(BaseAIAgent& ag);

virtual void onInitialize() override;

Status update() override;
};

class Wander : public Node {
public:
    float timeout;   // How many seconds to wait.
    float elapsed;    // Accumulated time.
    sf::Clock clock;  // Clock to measure delta time.
    Vec2 wanderSpot;
    BaseAIAgent& agent;
    Wander(BaseAIAgent& ag);
    virtual void onInitialize() override;

    virtual void reset() override;
    Status update() override;
};

class IsNearWall : public Node {

private:
    BaseAIAgent& agent;
public:

    IsNearWall(BaseAIAgent& ag) :agent(ag) {}
    Status update() override {
        
            if (agent.agent->getComponent<CVision>().seesWall) {
                // Store wall normal for tracing (simplified to vector difference)
                agent.wallNormal = Physics::GetWallNormal(agent.agent->getComponent<CTransform>().pos, agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos);
                std::cout << "Found Wall \n" << "Wall Normall is " << agent.wallNormal.x << " , " << agent.wallNormal.y << std::endl;
                return BH_SUCCESS;
            }
        return BH_FAILURE;
    }
}; 

class IsDoorVisible : public Node {
    IsDoorVisible(BaseAIAgent& agent) {}
    Status update() override {
        /*if (Physics::IsWallNearby(agent.transform.position)) {
            return BH_SUCCESS;
        }*/
        return BH_FAILURE;
    }
};


class WallTrace : public Node
{
public:
    WallTrace(BaseAIAgent& ag) :agent(ag){
        Name = "Move To Point";
        // std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
    }
private:
    BaseAIAgent& agent;
    float smallAngle = 5.0f * DEG2RAD;
    float wallTraceOffset = 10.0f;
    float speed = 2.0f;

    virtual void onInitialize() override {

      //  agent.initializeMoveToPoint(Waypoint);
    }

    virtual Status update() override {
        auto& transform = agent.agent->getComponent<CTransform>();
        Vec2 dir = { std::cos(agent.agent->getComponent<CTransform>().angle), std::sin(agent.agent->getComponent<CTransform>().angle) };
        // Calculate Vector from the wall
        bool wallRight = false;
        bool wallUp = false; 
        float xNormal;
        float yNormal;
        if (agent.agent->getComponent<CVision>().Wall)
        {
            xNormal = std::abs(agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos.x - transform.pos.x);
            yNormal = std::abs(agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos.y - transform.pos.y);
        }
        if (agent.agent->getComponent<CVision>().Wall)
        { 
            if (agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos.x > transform.pos.x)
            {
                wallRight = true;
            }

            if (agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos.y < transform.pos.y)
            {
                wallUp = true;
            }
        }
        /*
        Vec2 agentToWall;
        
        if (agent.agent->getComponent<CVision>().Wall)
        {

            agentToWall = agent.agent->getComponent<CVision>().Wall->getComponent<CTransform>().pos - transform.pos;
        }
        float side = dir.cross(agentToWall);
        agentToWall.normalize(); //NOTE: if anything happens, check if this is the problem.
        float dot = dir.dot(agentToWall);
            if (side<0) {
                wallRight = true;
            }
            
            if (dot > 0.5f)// Threshold: adjust if needed
            { 
                wallUp = true;
            }
            if (!wallRight) {
                // No wall rotate right follow the edge
                dir = dir.rotated(+smallAngle);

            }
            else {
                // Wall detected → rotate left slightly to stay near
                dir = dir.rotated(-smallAngle);
            }

            */

            // Move agent
            //transform.pos += dir * speed;// *Time::deltaTime;
            //agent.MoveToPoint(Waypoint);
        std::cout << "Right : " << wallRight << std::endl;
        std::cout << "Up : " << wallUp << std::endl;
        if (yNormal > xNormal)
        {
            std::cout << "Move Left/Right" << std::endl;
        }
        if (yNormal < xNormal)
        {
            std::cout << "Move Up/Down" << std::endl;
        }
            return BH_FAILURE;
 

    }
};

class EnterThroughDoor : public Node
{
public:
    EnterThroughDoor(BaseAIAgent& agent, Vec2& point) :greenAgent(agent), Waypoint(point) {
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
        // std::cout << "Moving to " << Waypoint.x<< " , " << Waypoint.y << std::endl;
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


class WallTraceToDoorSequence : public Selector {
public:
    WallTraceToDoorSequence(BaseAIAgent& agent) {

        StatefulSequence* wallTraceSequence = new StatefulSequence();
        wallTraceSequence->addChild(new IsNearWall(agent));
        wallTraceSequence->addChild(new WallTrace(agent));

        StatefulSequence* houseSearchSequence = new StatefulSequence();

        addChild(wallTraceSequence);
        //addChild(fleeSequence);
    }
};
class HouseSearch : public Node {
public:
    BaseAIAgent& agent;
    HouseSearch(BaseAIAgent& ag);
    virtual void onInitialize() override;
    virtual void reset() override;
    Status update() override;
};


class BlueAgentCombatSequence : public Selector
{
public:
    BlueAgentCombatSequence() { Name = "Combat Sequence"; }
    BlueAgentCombatSequence(BaseAIAgent& agent) {
        StatefulSequence* engageSequence = new StatefulSequence();
        engageSequence->addChild(new IsEnemyVisible(agent));
        engageSequence->addChild(new HasAmmo(agent));
        engageSequence->addChild(new TurnTowardsTarget(agent, 12));
        engageSequence->addChild(new EngageCombat(agent));
        engageSequence->addChild(new WaitForSeconds(agent, 0.5));
        engageSequence->addChild(new EngageCombat(agent));
        engageSequence->addChild(new WaitForSeconds(agent, 0.7));
        engageSequence->addChild(new EngageCombat(agent));
        engageSequence->addChild(new WaitForSeconds(agent, 2));

        Sequence* fleeSequence = new Sequence();
        fleeSequence->addChild(new IsEnemyVisible(agent));
        fleeSequence->addChild(new FleeToSafePosition(agent));
        addChild(engageSequence);
        addChild(fleeSequence);

    }
};



class CheckSeesFood : public Node {
    BaseAIAgent& ag;
public:
    CheckSeesFood(BaseAIAgent& a) :ag(a) { Name = "Check Sees Food"; }
    Status update() override {
        return ag.hasSeenFood
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
        return ag.hasSeenAmmo
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
        return ag.hasSeenCoin
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
        fetchFood->addChild(new MoveToPoint(ag, ag.FoodPosition));
      

        // Ammo branch
        Sequence* fetchAmmo = new Sequence();
        fetchAmmo->addChild(new CheckSeesAmmo(ag));
        fetchAmmo->addChild(new CheckNeedsAmmo(ag));
        fetchAmmo->addChild(new MoveToPoint(ag, ag.AmmoPosition));

        // Coin branch (always pick up if seen)
        Sequence* fetchCoin = new Sequence();
        fetchCoin->addChild(new CheckSeesCoin(ag));
        fetchCoin->addChild(new MoveToPoint(ag, ag.CoinPosition));
       

        addChild(fetchFood);
        addChild(fetchAmmo);
        addChild(fetchCoin);
    }
};



class PatrolSelector : public Selector {
public:
    PatrolSelector(){ Name = "Patrol Selector"; }
    PatrolSelector(BaseAIAgent& agent) {
      //  addChild(new LowHealth(agent));  // First, try healing
        addChild(new CombatSequence(agent)); //If Enemy is in Range, Engage in Combat
        addChild(new Patrol(agent)); //Patrol
    }
};
class SurvivalSelector : public Selector {
public:
    SurvivalSelector(){ Name = "Survival Selector"; }
    SurvivalSelector(BaseAIAgent& agent) {
        addChild(new LowHealth(agent));  // First, try healing
        addChild(new BlueAgentCombatSequence(agent)); //If Enemy is in Range, Engage in Combat
        addChild(new ItemFetchSelector(agent)); //Check for items
        addChild(new WallTraceToDoorSequence(agent)); //Check for items
        addChild(new Wander(agent)); //Patrol
    }
};
