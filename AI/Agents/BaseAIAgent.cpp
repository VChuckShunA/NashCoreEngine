#include "BaseAIAgent.h"
#include "../AIPlayroom.h"
#include "../Ammo.h"
#include "../Coin.h"
#include "../Health.h"
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

void BaseAIAgent::TakeDamage(std::shared_ptr<Entity> Instigator)
{
   if (!agent->getComponent<CVision>().Target)
    {
       room->TurnTowardsTarget(agent, Instigator, 2);
       agent->getComponent<CVision>().seesPlayer = true;
       agent->getComponent<CVision>().Target = Instigator;
    }
    //NOTE: Due to the iffy collision system, the Damage gets multiplied by 2
    if (health > 0)
    {
       // std::cout << "Previous Health " << health << std::endl;
        health -= baseDamageAmount;
      //  std::cout << "Current Health " << health << std::endl;
    }
   // else std::cout << "Player is dead "<< std::endl;
}

void BaseAIAgent::consumeFood()
{
    //This will cause an error if used by green agent
    if (hasFood()) {
        health = std::min(maxHealth, health + 50);
        //auto rit = std::find_if(
        //    room->inventory.rbegin(), room->inventory.rend(),
        //    [](Item* it) { return it->type == Item::ITM_HEALTH; }
        //);
        //if (rit != room->inventory.rend()) {
        //    room->inventory.erase(std::next(rit).base());
        //    //room->removeSlotAndCompact(rit);
        //    agent->getComponent<CAnimation>().animation = room->m_game->assets().getAnimation("Healing");
        //    room->UpdateInventoryUI();
        //}
      
        for (auto it = room->inventory.rbegin(); it != room->inventory.rend(); ++it) {     
            if (*it == nullptr) continue;
            if ((*it)->type == Item::ITM_HEALTH) {
                // Remove it
               // auto forward_it = std::next(it).base();
                //room->inventory.erase(forward_it);
                room->ResizeInventory();
                *it = nullptr;
                agent->getComponent<CAnimation>().animation = room->m_game->assets().getAnimation("Healing");
               // room->removeSlotAndCompact(removeCount);
                room->UpdateInventoryUI();
                std::cout << "INventory size " << room->inventory.size();
                break; // Exit after removing the first matching item from the end
            }
        }
        //  std::cout << "Consumed food. Health: " << health << std::endl;
    }
}

void BaseAIAgent::UpdateItemPosition()
{
    //if ((agent->getComponent<CVision>().Item)) ItemPosition = room->positionToGridCordinates(agent->getComponent<CVision>().Item->entity);

    //static_cast<Ammo>(agent->getComponent<CVision>().Item);
    //auto itemPtr = agent->getComponent<CVision>().Item;
   auto item = agent->getComponent<CVision>().Item.get();
    //ItemPosition= room->positionToGridCordinates(item->entity);
    if (item && item->type == Item::ITM_AMMO) {
        AmmoPosition = room->positionToGridCordinates(item->entity);
    }
    if (item && item->type == Item::ITM_HEALTH) {
        FoodPosition = room->positionToGridCordinates(item->entity);
    }
    if (item && item->type == Item::ITM_COIN) {
        CoinPosition = room->positionToGridCordinates(item->entity);
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
    constexpr float steerSpeed = 2.5f;
    auto& T = agent->getComponent<CTransform>();
    float current = fmod(T.angle + 360.0f, 360.0f);
    float target = fmod(targetAngle + 360.0f, 360.0f);

    if (current == target) return;

    // Compute positive‐only turnAngle ∈ [0,360)
    float turnAngle = fmod(target - current + 360.0f, 360.0f);

    if (turnAngle < 180.0f)
    {
        // Clockwise: current→current+steerSpeed, but clamp at target
        float nextAngle = current + steerSpeed;
        // If that step would pass “target” (in the circular sense):
        // Because turnAngle<180, target is “ahead” in the + direction.
        if (fmod(nextAngle, 360.0f) > target && turnAngle < steerSpeed)
        {
            T.angle = target;
            std::cout << "Clamped to target (clockwise)\n";
        }
        else
        {
            T.angle = nextAngle;
            std::cout << "Rotated +steerSpeed (clockwise)\n";
        }
    }
    else // turnAngle ≥ 180 → turn “the other way” (counter‐clockwise)
    {
        float nextAngle = current - steerSpeed;
        // When turnAngle>180, the “short path” goes negative toward target.
        // We check if we'd cross target by subtracting steerSpeed.
        float wrappedNext = fmod(nextAngle + 360.0f, 360.0f);
        // If the distance (in the CCW direction) is < steerSpeed, clamp:
        float ccwDist = 360.0f - turnAngle; // how far “below” current the target is
        if (ccwDist < steerSpeed)
        {
            T.angle = target;
            std::cout << "Clamped to target (CCW)\n";
        }
        else
        {
            T.angle = nextAngle;
            std::cout << "Rotated -steerSpeed (CCW)\n";
        }
    }

    // Finally force [0,360)
    T.angle = fmod(T.angle + 360.0f, 360.0f);
    /*

    float steerSpeed = 5;
    if (agent->getComponent<CTransform>().angle == targetAngle) return;

    float turnAngle = fmod(targetAngle - agent->getComponent<CTransform>().angle + 360, 360); // Normalize difference

    if (turnAngle < 180)
    {
        //clockwise
        float turnedAngled = agent->getComponent<CTransform>().angle + steerSpeed;
        if (turnedAngled > targetAngle)
        {
            float newSpeed = turnedAngled - agent->getComponent<CTransform>().angle;
            agent->getComponent<CTransform>().angle += newSpeed;
            std::cout << "Line 230" << std::endl;
        }
        else
        {
            agent->getComponent<CTransform>().angle += steerSpeed;
            std::cout << "Line 236" << std::endl;
        }
    }
    if (turnAngle > 180)
    {//counter clock wise
        float turnedAngled = agent->getComponent<CTransform>().angle - steerSpeed;
        if (turnedAngled < targetAngle)
        {
            float newSpeed = turnedAngled - agent->getComponent<CTransform>().angle;
            agent->getComponent<CTransform>().angle -= newSpeed;
            std::cout << "Line 246" << std::endl;
        }
        else
        {

            agent->getComponent<CTransform>().angle -= steerSpeed;

            std::cout << "Line 253" << std::endl;
        }
    }
    agent->getComponent<CTransform>().angle = fmod(agent->getComponent<CTransform>().angle + 360, 360);*/
}

bool BaseAIAgent::hasTarget()
{
    if (agent->getComponent<CVision>().Target)
        return true;
    return false;
}

bool BaseAIAgent::hasAmmo()
{
  //  std::cout << "Base Implementation hasAmmo" << std::endl;
    return true;
}

bool BaseAIAgent::hasFood()
{
//    std::cout << "Base Implementation hasFood" << std::endl;
    return false;
}

bool BaseAIAgent::needsFood()
{
//    std::cout << "Base Implementation needsFood" << std::endl;
    return true;
}

bool BaseAIAgent::needsAmmo()
{
  //  std::cout << "Base Implementation needsAmmo" << std::endl;
    return true;
}




IsEnemyVisible::IsEnemyVisible(BaseAIAgent& agent) : agent(agent)
{
    Name = "Is Enemy Visible";
}

Node::Status IsEnemyVisible::update()
{

    //agent.enemyState == VISIBLE
    //agent.agent->getComponent<CVision>().seesPlayer
    if (agent.agent->getComponent<CVision>().Target) {
        std::cout << "Sees target" << std::endl;
        return Status::BH_SUCCESS;
    }
    return Status::BH_FAILURE;
}

EngageCombat::EngageCombat(BaseAIAgent& agent) : agent(agent) { Name = "Engage Combat"; }

Node::Status EngageCombat::update()
{
    // Here you might return BH_RUNNING until the enemy is neutralized,
    // then return BH_SUCCESS (or BH_FAILURE if combat was interrupted).
    if (agent.hasAmmo())
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
    Name = "Turn Towards Target";
}


Node::Status TurnTowardsTarget::update()
{

    agent.room->TurnTowardsTarget(agent.agent, agent.agent->getComponent<CVision>().Target, randDev);
    return BH_SUCCESS;
}


FleeToSafePosition::FleeToSafePosition(BaseAIAgent& ag):agent(ag) {
            Name = "Flee To Safe Position";
        }

void FleeToSafePosition::onInitialize()
{
    Vec2  targetPosition = agent.room->positionToGridCordinates(agent.agent->getComponent<CVision>().Target);
    Vec2  playePosition = agent.room->positionToGridCordinates(agent.agent);
    oppositeDirection = agent.room->GetOppositeDirection(targetPosition, playePosition);
    safeSpot = agent.room->GetSafeSpot(oppositeDirection, playePosition);
    agent.initializeMoveToPoint(safeSpot);

}

Node::Status  FleeToSafePosition::update()
{
    if (!agent.destinationReached)
                {
                    agent.MoveToPoint(safeSpot);
                    std::cout << "Fleeing to " << safeSpot.x << " , "<< safeSpot.y << std::endl;
                    return BH_RUNNING; //Not reached destination 
                }
                else if (agent.destinationReached) {
                    return BH_SUCCESS; // Reached the point = success
                }
}

Wander::Wander(BaseAIAgent& ag):elapsed(0.0f), agent(ag),timeout(10.0f)
{
    Name = "Wandering";
}

void Wander::onInitialize()
{
    wanderSpot = agent.room->GetRandomWanderSpot();
    agent.initializeMoveToPoint(wanderSpot);
    clock.restart();
}

void Wander::reset() 
{
        elapsed = 0.0f;
        timeout = 10.0f;
        clock.restart();
        m_eStatus = BH_INVALID;
}

Node::Status Wander::update()
{
    elapsed = clock.getElapsedTime().asSeconds();
   

    std::cout << "Wander Update: dt = " << 0 << ", elapsed = " << elapsed << ", timeout = " << timeout << std::endl;


    if (agent.destinationReached) {
    //    std::cout << "Wander: Destination Reached. Returning BH_SUCCESS." << std::endl;
        return BH_SUCCESS;
    }
    if (elapsed >= timeout) {
   //     std::cout << "Wander: Timeout (" << elapsed << " >= " << timeout << "). Returning BH_SUCCESS." << std::endl;
        return BH_SUCCESS;
    }
    if (
        (agent.hasSeenAmmo && agent.needsAmmo() )||
        agent.hasSeenCoin  ||
        (agent.hasSeenFood && agent.needsFood()) ||
        agent.agent->getComponent<CVision>().seesPlayer ||
        agent.agent->getComponent<CVision>().seesWall || agent.hasSeenWall ||
        (agent.agent->getComponent<CVision>().seesFood && agent.needsFood()) ||
        agent.agent->getComponent<CVision>().seesCoin ||
        (agent.agent->getComponent<CVision>().seesAmmo && agent.needsAmmo()) ||
        agent.agent->getComponent<CVision>().Target) {
       // std::cout << "Wander: Interrupted by Perception. Returning BH_SUCCESS." << std::endl;
        // You might want to log specifically *what* caused the interruption
     /*   if (agent.hasSeenAmmo) std::cout << "  - Saw Ammo" << std::endl;
        if (agent.hasSeenCoin) std::cout << "  - Saw Coin" << std::endl;
        if (agent.hasSeenFood) std::cout << "  - Saw Food" << std::endl;
        if (agent.agent->getComponent<CVision>().seesPlayer) std::cout << "  - Saw Player" << std::endl;*/
        return BH_SUCCESS; // Reached the point = success
    } 

    agent.MoveToPoint(wanderSpot);
  //  std::cout << "Wandering (Still Running)" << std::endl;
        return BH_RUNNING; //Not reached destination 
    

}

WallTrace::WallTrace(BaseAIAgent& ag) :agent(ag) {
    Name = "Move To Point";
    // std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
}

void WallTrace::onInitialize()
{
    //agent.initializeMoveToPoint(upPosition);
}

Node::Status WallTrace::update()
{
    auto& transform = agent.agent->getComponent<CTransform>();
    auto& vision = agent.agent->getComponent<CVision>();
    auto& tracker = agent.agent->getComponent<CWallTracker>(); 
  //  Vec2 normal = tracker.lastHitNormal;

    Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
    upPosition = agentTile + Vec2(0, 1);
    downPosition = agentTile + Vec2(0, -1);
    rightPosition = agentTile + Vec2(1, 0);
    leftPosition = agentTile + Vec2(-1, 0);
    diagDownLeft = agentTile + Vec2(-1,-1);
    diagDownRight = agentTile + Vec2(1, -1);
    diagUpLeft = agentTile + Vec2(-1, 1);
    diagUpRight = agentTile + Vec2(1, 1);
    float angleRad = transform.angle * (std::numbers::pi / 180.0f);
    Vec2  forward = { std::cos(angleRad), std::sin(angleRad) };
    Vec2 dir = { std::cos(transform.angle), std::sin(transform.angle) };
    Vec2 wallTile;
    Vec2 agentToWall = wallTile - agentTile;
    float side = dir.cross(agentToWall);
    bool wallRight = (side < 0);
    bool wallUp = false; 
    agentToWall.normalize();
    float dot = dir.dot(agentToWall);

    if (abs(agentToWall.x) > abs(agentToWall.y)) {
        // Horizontal relation → wall is mostly left/right
        wallUp = false;
    }
    else {
        // Vertical relation → wall is mostly up/down
        wallUp = true;
    }
    // Calculate Vector from the wall
   // Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
    if (vision.NearestBrick)
    {
        wallTile= agent.room->positionToGridCordinates(vision.NearestBrick);
    }


    if (vision.seesWall ||tracker.wallLeft|| tracker.wallRight) {
        std::cout << "Wall detected. Initiating trace!" << std::endl;
        std::cout << "Sees Wall: " << vision.seesWall<<"\nwallLeft : "<<tracker.wallLeft<<"\nwallRight: "<<tracker.wallRight<< std::endl;
        
       

        if (vision.seesWall && !(tracker.wallLeft || tracker.wallRight))
        {
            //move towards the wall
            Vec2 gridStep;
            if (std::abs(forward.x) > std::abs(forward.y)) {
                gridStep = (forward.x > 0) ? Vec2(1, 0) : Vec2(-1, 0);
            }
            else {
                gridStep = (forward.y > 0) ? Vec2(0, 1) : Vec2(0, -1);
            }

            Vec2 targetTile = agentTile + gridStep;
            if (agent.room->navmesh.navMesh[targetTile.x][targetTile.y].walkable)
            {

                agent.MoveToPoint(targetTile);
                return BH_RUNNING;
            }
        }
        if (tracker.wallLeft || tracker.wallRight)
        {

            std::cout << "Wall detected. On Left or RIght!" << std::endl;

            std::cout << "Forward : " << forward.x<< " , "<< forward.y << std::endl;
            std::cout << "Direction : " << dir.x<< " , " <<dir.y<< std::endl;
            std::cout << "Wall Tile : " << wallTile.x<< " , "<< wallTile.y<<std::endl;
            std::cout << "Agent To Wall : " <<agentToWall.x << " , "<<agentToWall.y << std::endl;
            std::cout << "Wall to right of the agent (SIDE) : " << wallRight<< std::endl;
            std::cout << "Wall ABOVE the agent (SIDE) : " << wallUp << std::endl;

            if (abs(agentToWall.x) > abs(agentToWall.y)) {
                // Wall is mostly left/right relative to agent
                if (agentToWall.x > 0) {
                    // Wall is to the right → move right
                    agent.MoveToPoint(rightPosition);
                    return BH_RUNNING;
                }
                else {
                    // Wall is to the left → move left
                    agent.MoveToPoint(leftPosition);
                    return BH_RUNNING;
                }
            }
            else {
                // Wall is mostly up/down relative to agent
                if (agentToWall.y > 0) {
                    // Wall is above → move up
                    agent.MoveToPoint(upPosition);
                    return BH_RUNNING;
                }
                else {
                    // Wall is below → move down

                    agent.MoveToPoint(downPosition);
                    return BH_RUNNING;
                }
            }
            //return BH_RUNNING;
        }
        //return BH_RUNNING;
        agent.agent->getComponent<CVision>().NearestBrick = agent.agent->getComponent<CVision>().LastKnownBrick;
       // return BH_SUCCESS;
    }
    

    std::cout << "No Conditions are met, returning Failure!" << std::endl;
    return BH_RUNNING;

/*
auto& transform = agent.agent->getComponent<CTransform>();
auto& vision = agent.agent->getComponent<CVision>();
auto& tracker = agent.agent->getComponent<CWallTracker>();
Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
upPosition = agentTile + Vec2(0, 1);
downPosition = agentTile + Vec2(0, -1);
rightPosition = agentTile + Vec2(1, 0);
leftPosition = agentTile + Vec2(-1, 0);
float angleRad = transform.angle * (std::numbers::pi / 180.0f);
Vec2  forward = { std::cos(angleRad), std::sin(angleRad) };
Vec2 dir = { std::cos(transform.angle), std::sin(transform.angle) };
Vec2 wallTile;
Vec2 agentToWall = wallTile - agentTile;
float side = dir.cross(agentToWall);
bool wallRight = (side < 0);
bool wallUp = false;
agentToWall.normalize();
float dot = dir.dot(agentToWall);

if (abs(agentToWall.x) > abs(agentToWall.y)) {
    // Horizontal relation → wall is mostly left/right
    wallUp = false;
}
else {
    // Vertical relation → wall is mostly up/down
    wallUp = true;
}
    // WallUp detection based on relative position
    if (abs(agentToWall.x) >= abs(agentToWall.y)) {
        std::cout << "line 439!" << std::endl;
        if (agentToWall.x >= 0) {
            transform.pos.x += speed;
            std::cout << "Moving Right" << std::endl;
            targetPosition = rightPosition;
           // agent.initializeMoveToPoint(targetPosition);
           // agent.room->TurnTowardsPosition(agent.agent,targetPosition, 0);
            //agent.MoveToPoint(targetPosition);
            return BH_RUNNING;
        }
        else {
            transform.pos.x -= speed;
            std::cout << "Moving Left" << std::endl;
            targetPosition = leftPosition;
         //   agent.initializeMoveToPoint(targetPosition);
            //agent.room->TurnTowardsPosition(agent.agent, targetPosition, 0);
           // agent.MoveToPoint(targetPosition);
            return BH_RUNNING;
        }
    }
    else {
        std::cout << "line 456!" << std::endl;
        if (agentToWall.y >= 0) {
            transform.pos.y += speed;
            std::cout << "Moving Up" << std::endl;
            targetPosition = upPosition;
           // agent.initializeMoveToPoint(targetPosition);
            //agent.room->TurnTowardsPosition(agent.agent, targetPosition, 0);
            //agent.MoveToPoint(targetPosition);
            return BH_RUNNING;
        }
        else {
            transform.pos.y -= speed;
            std::cout << "Moving Down" << std::endl;
            targetPosition = downPosition;
            //agent.initializeMoveToPoint(targetPosition);
            //agent.room->TurnTowardsPosition(agent.agent, targetPosition, 0);
            //agent.MoveToPoint(targetPosition);
            return BH_RUNNING;
        }
    }*/
}