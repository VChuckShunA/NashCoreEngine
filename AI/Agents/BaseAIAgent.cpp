#include "BaseAIAgent.h"
#include "../AIPlayroom.h"
#include "../Ammo.h"
#include "../Coin.h"
#include "../Health.h"
#include "../House/HouseGenerator.h"
#include "../House/House.h"
#include "../Pathfinding/BoustrophedonPathfinder.h"
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

void BaseAIAgent::FollowPath()
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
                std::cout << "131" << std::endl;
                currentpath.erase(currentpath.begin());
            }
            //TODO: Find a cleaner a way to do this
            if (AgentCTransform.x < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move right
                std::cout << "137" << std::endl;
                AgentCTransform.x = AgentCTransform.x + AISpeed;
                left = false;
                right = true;
            }if (AgentCTransform.x > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).x)
            {
                //move left
                std::cout << "144" << std::endl;

                AgentCTransform.x = AgentCTransform.x - AISpeed;
                left = true;
                right = false;
            }
            if (AgentCTransform.y < room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Down

                std::cout << "155" << std::endl;
                AgentCTransform.y = AgentCTransform.y + AISpeed;
                down = true;
                up = false;

            }if (AgentCTransform.y > room->gridToMidPixel(currentpath.front().x, currentpath.front().y, agent).y)
            {
                //move Up

                std::cout << "162" << std::endl;

                AgentCTransform.y = AgentCTransform.y - AISpeed;
                up = true;
                down = false;
            }
            //0=right,90=down,180 =left, 270=up

            if (up && left)
            {
                //entity->getComponent<CTransform>().angle = 225;
                std::cout << "174" << std::endl;
                steer(225);
            }
            if (up && right)
            {
                //entity->getComponent<CTransform>().angle = 315;
                std::cout << "180" << std::endl;
                steer(315);
            }
            if (down && left)
            {
                // entity->getComponent<CTransform>().angle = 135;
                std::cout << "186" << std::endl;
                steer(135);
            }
            if (down && right)
            {
                // entity->getComponent<CTransform>().angle = 45;
                std::cout << "192" << std::endl;
                steer(45);
            }
            if (up)
            {
                //entity->getComponent<CTransform>().angle = 270;
                std::cout << "198" << std::endl;
                steer(270);
            }
            if (down)
            {
                //entity->getComponent<CTransform>().angle = 90;

                std::cout << "205" << std::endl;
                steer(90);
            }
            if (left)
            {
                // entity->getComponent<CTransform>().angle = 180;

                std::cout << "212" << std::endl;
                steer(180);
            }
            if (right)
            {
                // entity->getComponent<CTransform>().angle = 0;
                std::cout << "218" << std::endl;
                steer(0);
            }

        }
        if (currentpath.empty())
        {
            //     std::cout << "GreenAgent 164" << std::endl;
            std::cout << "226" << std::endl;
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
          //  std::cout << "Clamped to target (clockwise)\n";
        }
        else
        {
            T.angle = nextAngle;
          //  std::cout << "Rotated +steerSpeed (clockwise)\n";
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
          // std::cout << "Clamped to target (CCW)\n";
        }
        else
        {
            T.angle = nextAngle;
         //   std::cout << "Rotated -steerSpeed (CCW)\n";
        }
    }

    // Finally force [0,360)
    T.angle = fmod(T.angle + 360.0f, 360.0f);
   
}

static float normalize360(float a) {
    a = fmod(a, 360.0f);
    if (a < 0.0f) a += 360.0f;
    return a;
}


void BaseAIAgent::turnLeft() {
    auto& T = agent->getComponent<CTransform>();
    T.angle = normalize360(T.angle + 1);
}

void BaseAIAgent::turnRight() {
    auto& T = agent->getComponent<CTransform>();
    T.angle = normalize360(T.angle - 1);
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
                    agent.FollowPath();
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
   

  //  std::cout << "Wander Update: dt = " << 0 << ", elapsed = " << elapsed << ", timeout = " << timeout << std::endl;


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

    agent.FollowPath();
  //  std::cout << "Wandering (Still Running)" << std::endl;
        return BH_RUNNING; //Not reached destination 
    

}

WallTrace::WallTrace(BaseAIAgent& ag) :agent(ag) {
    Name = "Wall Trace";
    // std::cout << "Moving Way Point" << Waypoint.x << " , " << Waypoint.y << std::endl;
}

void WallTrace::onInitialize()
{
    agent.currentHouse = HouseGenerator::FindHouseByID(agent.agent->getComponent<CVision>().NearestBrick->m_buildingID);
    Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
    doorPosition = agent.currentHouse->GetClosestMainDoor(agentTile);
    agent.room->isScanningWalls = false;
    agent.initializeMoveToPoint(doorPosition);
}

Vec2 WallTrace::getLastHitNormal(Vec2 wallTile, Vec2 agentTile)
{
    // 1) Get the raw direction from agent to brick
    Vec2 raw = wallTile - agentTile;
    Vec2 lastHitNormal;
    // 2) Decide if this is mostly a horizontal or vertical contact
    if (std::abs(raw.x) > std::abs(raw.y)) {
        // The brick lies directly to our LEFT or RIGHT
        // Normal points from the brick toward the agent, i.e. opposite of raw.x
        float sx = (raw.x > 0 ? -1.0f : +1.0f);
        lastHitNormal = Vec2(sx, 0);   // (–1,0) if brick is to our right; (+1,0) if brick is to our left
    }
    else {
        // The brick lies directly ABOVE or BELOW
        float sy = (raw.y > 0 ? -1.0f : +1.0f);
        lastHitNormal = Vec2(0, sy);   // (0,–1) if brick is above; (0,+1) if brick is below
    }

    return lastHitNormal;
}

Node::Status WallTrace::update()
{
    //std::cout << "Wall Trace: Update" << testInt++ << std::endl;
    Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
    agent.room->isScanningWalls = false;
    if(agent.room->navmesh.navMesh[agentTile.x][agentTile.y].insideHouse) return BH_SUCCESS;
    /*  std::cout << "THIS SHIT IS RUNNING"<< std::endl;
    auto& transform = agent.agent->getComponent<CTransform>();
    auto& vision = agent.agent->getComponent<CVision>();
    auto& tracker = agent.agent->getComponent<CWallTracker>();
    auto& navmesh = agent.room->navmesh.navMesh;


    std::cout << "WALL ID " << vision.NearestBrick->m_buildingID << std::endl;*/
    if (agent.destinationReached) {
        //    std::cout << "Wander: Destination Reached. Returning BH_SUCCESS." << std::endl;
        return BH_SUCCESS;
    }

    agent.FollowPath();
    //  std::cout << "Wandering (Still Running)" << std::endl;
    return BH_RUNNING; //Not reached destination 


    ////  Vec2 normal = tracker.lastHitNormal;
    //Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
    //upPosition = agentTile + Vec2(0, 1);
    //downPosition = agentTile + Vec2(0, -1);
    //rightPosition = agentTile + Vec2(1, 0);
    //leftPosition = agentTile + Vec2(-1, 0);
    //diagDownLeft = agentTile + Vec2(-1, -1);
    //diagDownRight = agentTile + Vec2(1, -1);
    //diagUpLeft = agentTile + Vec2(-1, 1);
    //diagUpRight = agentTile + Vec2(1, 1);
    //float angleRad = transform.angle * (std::numbers::pi / 180.0f);
    //Vec2 forward = { std::cos(angleRad), std::sin(angleRad) };
    //Vec2 dir = { std::cos(transform.angle), std::sin(transform.angle) };
    //Vec2 wallTile;
    //Vec2 agentToWall = wallTile - agentTile;
    //float side = dir.cross(agentToWall);
    //bool wallRight = (side < 0);
    //bool wallUp = false;
    //agentToWall.normalize();
    //float dot = dir.dot(agentToWall);
    //
    //bool moveLeft = false;
    //bool moveRight = false;
    //bool moveUp = false;
    //bool moveDown = false;
    //// Compute direction to wall
    //Vec2 dirToWall = wallTile - agentTile;
    //dirToWall.normalize();
    //if (vision.seesDoor) return BH_SUCCESS;
    // Early-out if we see no wall
   



}

HouseSearch::HouseSearch(BaseAIAgent& ag) :agent(ag)
{
}

void HouseSearch::onInitialize()
{
    pathGenerated = false;   // reset flag
    agent.destinationReached = false;
   
    if (agent.currentHouse)
    {
        auto coverCells = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);
        std::vector<Vec2> fullPath;
        fullPath.clear();
        Vec2 last = agent.room->positionToGridCordinates(agent.agent);
        for (auto& target : coverCells) {
            auto sub = agent.room->navmesh.FindPath(last, target);
            if (!sub.empty()) {
                fullPath.insert(fullPath.end(), sub.begin() + 1, sub.end());
                last = target;
            }
        }
        agent.currentpath = fullPath;
        /*
        agent.currentpath = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);
        agent.destinationReached = false;*/

        for (const Vec2& point : agent.currentpath)
        {
            std::cout << "Path : "<< point.x<< " , " << point.y<< std::endl;
        }
        pathGenerated = true;
    }
}

void HouseSearch::reset()
{
}

Node::Status HouseSearch::update()
{
    std::cout << "House Search Room count : " << agent.currentHouse->rooms.size() << std::endl;
    if (!agent.currentHouse)
    {
  //      std::cout << "No Current House" << std::endl;
        return BH_FAILURE;
    } if (agent.currentHouse)
    {
     //   std::cout << "Current House exists" << std::endl;
     //   std::cout << "Bounds \nLeft :"<< agent.currentHouse->bounds.left << 
    //        " \nTop : "<< agent.currentHouse->bounds.top<< 
     //       " \nWidth : " << agent.currentHouse->bounds.width<<" \nHeight : "<< agent.currentHouse->bounds.height << std::endl;
   //     std::cout << "Current House exists" << std::endl;
        if (!pathGenerated) {
            auto coverCells = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);
            std::vector<Vec2> fullPath;
            fullPath.clear();
            Vec2 last = agent.room->positionToGridCordinates(agent.agent);
            for (auto& target : coverCells) {
                auto sub = agent.room->navmesh.FindPath(last, target);
                if (!sub.empty()) {
                    fullPath.insert(fullPath.end(), sub.begin() + 1, sub.end());
                    last = target;
                }
            }
            agent.currentpath = fullPath;
            /*
            agent.currentpath = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);
            agent.destinationReached = false;*/

            for (const Vec2& point : agent.currentpath)
            {
                std::cout << "Path : " << point.x << " , " << point.y << std::endl;
            }
            pathGenerated = true;
           
           /* agent.currentpath = BoustrophedonPathfinder::GeneratePath(
                agent.currentHouse->bounds,
                agent.room->navmesh.navMesh
            );*/
            std::cout << "Generated path of size " << agent.currentpath.size() << "\n";
        }

        for (const Vec2& point : agent.currentpath)
        {
            std::cout << "Path : " << point.x << " , " << point.y << std::endl;
        }
    }
   /* agent.currentpath = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);
    agent.destinationReached = false;*/

    for (const Vec2& point : agent.currentpath)
    {
        std::cout << "Path : " << point.x << " , " << point.y << std::endl;
    }

 //   std::cout << "Generated path size: " << agent.currentpath.size() << std::endl;
 //   std::cout << "Timer: " << timer++ << std::endl;

    if (!agent.destinationReached)
    {
        std::cout << "We are RUNNING" << std::endl;
        agent.FollowPath();
        return BH_RUNNING; //Not reached destination 
    }
    else if (agent.destinationReached) {
        return BH_SUCCESS; // Reached the point = success
    }
}