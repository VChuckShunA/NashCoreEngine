#include "AIPlayroom.h"
#include "../Action.h"
#include "../SceneMenu.h"
#include "../Vec2.h"
#include "../Physics.h"
#include "Agents/GreenAgent.h"
#include "Agents/BlueAgent.h"
#include "House/HouseGenerator.h"
#include "Health.h"
#include "Ammo.h"
#include "Coin.h"
#include "Item.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include<numbers>
#include <random>
AIPlayroom::AIPlayroom(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
    init(levelPath);

    //inventory.resize(INVENTORY_SIZE, nullptr);

}

void AIPlayroom::init(const std::string& levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid

    // Register all other gameplay Actions
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");
    registerAction(sf::Keyboard::K, "MoveAgent");

    m_gridText.setCharacterSize(12);
    // m_gridText.setFont(m_game->assets().getFont("Mario"));
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    navmesh.initializeNavMesh();
    //Spawn AI
    HouseGenerator::GenerateWareHouse(navmesh, *this);
    HouseGenerator::GenerateLHouse(navmesh, *this);
    HouseGenerator::GenerateEightHouse(navmesh, *this);
    HouseGenerator::GenerateMansion(navmesh, *this);
   
   // SpawnEnemies();
    auto p1 = m_entityManager.addEntity("player");
    p1->addComponent<CAnimation>(m_game->assets().getAnimation("BlueAgent"), true);
    p1->addComponent<CTransform>(
        gridToMidPixel(1, 19, p1),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    p1->addComponent<CBoundingBox>(Vec2(64, 64));
    p1->addComponent<CVision>();
    p1->addComponent<CWallTracker>();
    /*
    auto item1 = m_entityManager.addEntity("health");
    item1->addComponent<CAnimation>(m_game->assets().getAnimation("FirstAid"), true);
    item1->addComponent<CTransform>(
        gridToMidPixel(10, 3, item1),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item1->addComponent<CBoundingBox>(Vec2(64, 64));
    //std::shared_ptr<Health> healthItem = std::make_shared<Health>(item1);

    auto item2 = m_entityManager.addEntity("ammo");
    item2->addComponent<CAnimation>(m_game->assets().getAnimation("Bullets"), true);
    item2->addComponent<CTransform>(
        gridToMidPixel(4, 3, item2),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item2->addComponent<CBoundingBox>(Vec2(64, 64));
  //  std::shared_ptr<Ammo> ammoItem = std::make_shared<Ammo>(item2);

    auto item3 = m_entityManager.addEntity("coin");
    item3->addComponent<CAnimation>(m_game->assets().getAnimation("CoinSpin"), true);
    item3->addComponent<CTransform>(
        gridToMidPixel(17, 5, item3),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item3->addComponent<CBoundingBox>(Vec2(64, 64));
    //std::shared_ptr<Coin> coinItem = std::make_shared<Coin>(item3);
    auto item4 = m_entityManager.addEntity("ammo");
    item4->addComponent<CAnimation>(m_game->assets().getAnimation("Bullets"), true);
    item4->addComponent<CTransform>(
        gridToMidPixel(19, 10, item4),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item4->addComponent<CBoundingBox>(Vec2(64, 64));

    auto item5 = m_entityManager.addEntity("health");
    item5->addComponent<CAnimation>(m_game->assets().getAnimation("FirstAid"), true);
    item5->addComponent<CTransform>(
        gridToMidPixel(11, 3, item5),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item5->addComponent<CBoundingBox>(Vec2(64, 64));


    auto item6 = m_entityManager.addEntity("health");
    item6->addComponent<CAnimation>(m_game->assets().getAnimation("FirstAid"), true);
    item6->addComponent<CTransform>(
        gridToMidPixel(4, 11, item6),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item6->addComponent<CBoundingBox>(Vec2(64, 64));

    auto item7 = m_entityManager.addEntity("health");
    item7->addComponent<CAnimation>(m_game->assets().getAnimation("FirstAid"), true);
    item7->addComponent<CTransform>(
        gridToMidPixel(12, 3, item7),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    item7->addComponent<CBoundingBox>(Vec2(64, 64));
    */
    //Health(healthItem);
    agents.emplace_back(make_unique<BlueAgent>(p1, this));
    playerPtr = static_cast<BlueAgent*>(agents.back().get());
    /*
    items.emplace_back(std::make_shared<Health>(item1));
    items.emplace_back(std::make_shared<Ammo>(item2));
    items.emplace_back(std::make_shared<Coin>(item3));
    items.emplace_back(std::make_shared<Ammo>(item4));
    items.emplace_back(std::make_shared<Health>(item5));
    items.emplace_back(std::make_shared<Health>(item6));
    items.emplace_back(std::make_shared<Health>(item7));*/
}


Vec2 AIPlayroom::gridToMidPixel(float gridX, float gridY, const std::shared_ptr<Entity>& entity) {
    // This function takes in a grid (x,y) position and an Entity
    // Return a vec2 indicating where the CENTER position of the Entity should be
    // You must use the Entity's Animation size to position it correctly
    // The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    // The bottom-left corner of the Animation should align with the bottom left of the grid cell

    auto entitySize = entity->getComponent<CAnimation>().animation.getSize();
 
    return {
            gridX * m_gridSize.x + entitySize.x / 2.0f,
            height() - gridY * m_gridSize.y - entitySize.y / 2.0f
    };
}



void AIPlayroom::RemoveAgent(BaseAIAgent* ptr)
{
    agents.erase(
        std::remove_if(agents.begin(), agents.end(),
            [&](auto const& up) { return up.get() == ptr; }),
        agents.end()
    );
    ptr->agent->destroy();
    //delete ptr;
    //ptr = nullptr;
}

void AIPlayroom::RemoveItem(Item* ptr)
{
    auto it = std::find_if(items.begin(), items.end(),
        [&](auto const& up) { return up.get() == ptr; });
    if (it != items.end()) {
        if (ptr->type == Item::ITM_COIN)
        {
            playerPtr->hasSeenCoin = false;
            items.erase(it);     // deletes no object—because ptr is !null
            return;
        }
        // move the unique_ptr into the first empty slot:
        for (auto& slot : inventory) {
            if (!slot) {
                slot = std::move(*it);    // transfer ownership
                items.erase(it);     // deletes no object—because ptr is !null
                if (ptr->type == Item::ITM_AMMO)
                {
                    playerPtr->hasSeenAmmo = false;
                }
                if (ptr->type == Item::ITM_HEALTH)
                {
                    playerPtr->hasSeenFood = false;
                }
                UpdateInventoryUI();
                break;
            }
        }
    }

   
}

void AIPlayroom::spawnBullet(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target) {
    //spawning bullet
    if (target)
    {
        if (entity.get()->tag() == "player")
        {
            ammoCount--;
            if (!ammoCount % 3)
            {
                std::cout << "Remove Ammo Now!" << std::endl;
                for (auto it = inventory.rbegin(); it != inventory.rend(); ++it) {
                    if (*it == nullptr) continue;
                    if ((*it)->type == Item::ITM_AMMO) {
                        // Remove it
                        ResizeInventory();
                        *it = nullptr;
                        UpdateInventoryUI();
                        std::cout << "INventory size " << inventory.size();
                        break; // Exit after removing the first matching item from the end
                    }
                }

            }
        }
    auto bullet = m_entityManager.addEntity("bullet", entity);
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation("Buster"), true);

    //Setting bullet transforms
    bullet->addComponent<CTransform>(
                entity->getComponent<CTransform>().pos ,    //position
                Vec2(0, 0),                                 //previous position
                Vec2{1.0,1.0},                              //scale
                entity->getComponent<CTransform>().angle    //angle
    ); 
  //  float angle = 90 * (std::numbers::pi / 180.0f);
     // Retrieve the position and angle from the entity
    //auto& targetTransform = target->getComponent<CTransform>();

    // shoot straight (at your own angle)
    auto& entityTransform = entity->getComponent<CTransform>();
    Vec2 position = entityTransform.pos;
    float angleDegrees = entityTransform.angle;
    float angleRadians = angleDegrees * (std::numbers::pi / 180.0f);


    //Turn towards Target
    //NOTE: If you get pointed to this after player died,it's b because the target is a nullptr
    //TODO: FIX IT SO THAT THE IT ONLY FIRES WHILE THE TARGET EXISTS
   
    //float deltaX = targetTransform.pos.x - entity->getComponent<CTransform>().pos.x;
    //float deltaY = targetTransform.pos.y - entity->getComponent<CTransform>().pos.y;
    //float angleRadians = std::atan2(deltaY, deltaX); // Angle in radians
    //float angleDegrees = angleRadians * (180.0f / std::numbers::pi); // Convert to degrees if needed

  //  steer(entity, angleRadians);
    // Convert angle from degrees to radians
   // float angleRadians = 90 * (std::numbers::pi / 180.0f);

    // Define bullet speed
    float speed = 2.0f;
    bullet->getComponent<CTransform>().velocity.x = cos(angleRadians) * speed;
    bullet->getComponent<CTransform>().velocity.y = sin(angleRadians) * speed;
    bullet->addComponent<CLifespan>(120, m_currentFrame);
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
    }

    
}

void AIPlayroom::update() {
    m_entityManager.update();

    // implement pause functionality
    if (!m_paused) {
        sMovement();
        sLifespan();
        sCollision();
        sVisionCone();
        RunBehaviourTrees();
        std::cout << "Ammo Count : " << ammoCount << std::endl;
        ////sVisionCone();
        //std::cout << "Has Seen Food : " << playerPtr->hasSeenFood << std::endl;
        //std::cout << "Has Seen Ammo : " << playerPtr->hasSeenAmmo << std::endl;
        //std::cout << "Has Seen Coin : " << playerPtr->hasSeenCoin << std::endl;
        //std::cout << "Food Position : " << playerPtr->FoodPosition.x <<" , "<<playerPtr->FoodPosition.y << std::endl;
        //std::cout << "Ammo Position : " << playerPtr->AmmoPosition.x << " , " << playerPtr->AmmoPosition.y << std::endl;
        //bool needfood= playerPtr->needsFood();
        //bool needammo = playerPtr->needsAmmo();
        //std::cout << "Needs Food : " << needfood << std::endl;
        //std::cout << "Needs Ammo : " << needammo << std::endl;
        m_currentFrame++;
    }
    sAnimation();
    sRender();
}

bool AIPlayroom::LineOfSight(const Vec2& A, const Vec2& P)
{
    for (auto& brick : m_entityManager.getEntities("Brick")) {
        auto& T = brick->getComponent<CTransform>();
        auto& B = brick->getComponent<CBoundingBox>();
        float xmin = T.pos.x - B.halfSize.x,
            xmax = T.pos.x + B.halfSize.x;
        float ymin = T.pos.y - B.halfSize.y,
            ymax = T.pos.y + B.halfSize.y;
        if (liangBarsky(A.x, A.y, P.x, P.y, xmin, ymin, xmax, ymax))
            return false;
    }
    return true;
}

bool AIPlayroom::liangBarsky(float x0, float y0, float x1, float y1, float xmin, float ymin, float xmax, float ymax)
{
    float dx = x1 - x0, dy = y1 - y0;
    float p[4] = { -dx, dx, -dy, dy };
    float q[4] = { x0 - xmin, xmax - x0, y0 - ymin, ymax - y0 };
    float u1 = 0.0f, u2 = 1.0f;
    for (int i = 0; i < 4; ++i) {
        if (p[i] == 0) {
            if (q[i] < 0) return false;
        }
        else {
            float t = q[i] / p[i];
            if (p[i] < 0) u1 = std::max(u1, t);
            else          u2 = std::min(u2, t);
            if (u1 > u2)  return false;
        }
    }
    return true;;
}

Vec2 AIPlayroom::positionToGridCordinates(const std::shared_ptr<Entity>& entity)
{

    return { (entity->getComponent<CTransform>().pos.x / 64) ,
        ((m_game->window().getSize().y - (entity->getComponent<CTransform>().pos.y - 1)) / 64) };
  
}

void AIPlayroom::MoveEntity(const std::shared_ptr<Entity>& entity, std::vector<Vec2>& path)
{
    int AISpeed = 1;
    bool destinationReached = false;
    bool up = false, down = false, left = false, right = false;
    float angleToWaypoint;
    Vec2 A = Vec2((int)positionToGridCordinates(entity).x, (int)positionToGridCordinates(entity).y);
    Vec2 B= Vec2(path.front().x, path.front().y);
 
     //0=right,90=down,180 =left, 270=up
    Vec2 distanceBetween;
    if (!path.empty()) {
        if (!destinationReached)
        {
            distanceBetween = Vec2(abs(entity->getComponent<CTransform>().pos.x - gridToMidPixel(path.front().x, path.front().y, entity).x), abs(entity->getComponent<CTransform>().pos.y - gridToMidPixel(path.front().x, path.front().y, entity).y));
            if (entity->getComponent<CTransform>().pos == Vec2(gridToMidPixel(path.back().x, path.back().y, entity)))
            {
                destinationReached=true;
            }
            
           
            if (distanceBetween.x<5&&distanceBetween.y<5)
            {
                path.erase(path.begin());
            }
            //TODO: Find a cleaner a way to do this
            if (entity->getComponent<CTransform>().pos.x < gridToMidPixel(path.front().x, path.front().y, entity).x)
            {
                //move Left
                entity->getComponent<CTransform>().pos.x= entity->getComponent<CTransform>().pos.x+AISpeed;
                left = false;
                right = true;
            }if (entity->getComponent<CTransform>().pos.x > gridToMidPixel(path.front().x, path.front().y, entity).x)
            {
                //move Right
                entity->getComponent<CTransform>().pos.x=entity->getComponent<CTransform>().pos.x-AISpeed;
                left = true;
                right = false;
            }
            if (entity->getComponent<CTransform>().pos.y < gridToMidPixel(path.front().x, path.front().y, entity).y)
            {
                //move Down
                entity->getComponent<CTransform>().pos.y = entity->getComponent<CTransform>().pos.y+AISpeed;
                down = true;
                up = false;
               
            }if (entity->getComponent<CTransform>().pos.y > gridToMidPixel(path.front().x, path.front().y, entity).y)
            {
                //move Up
                entity->getComponent<CTransform>().pos.y = entity->getComponent<CTransform>().pos.y-AISpeed;
                up = true;
                down = false;
            }
            //0=right,90=down,180 =left, 270=up

            if (up && left)
            {
                //entity->getComponent<CTransform>().angle = 225;
                steer(entity,225);
            }
            if (up && right)
            {
                //entity->getComponent<CTransform>().angle = 315;
                steer(entity, 315);
            }
            if (down && left)
            {
               // entity->getComponent<CTransform>().angle = 135;
                steer(entity, 135);
            }
            if (down && right)
            {
               // entity->getComponent<CTransform>().angle = 45;
                steer(entity, 45);
            }
            if (up)
            {
                //entity->getComponent<CTransform>().angle = 270;
                steer(entity, 270);
            }
            if (down)
            {
                //entity->getComponent<CTransform>().angle = 90;
                steer(entity, 90);
            }
            if (left)
            {
               // entity->getComponent<CTransform>().angle = 180;
                steer(entity, 180);
            }
            if (right)
            {
               // entity->getComponent<CTransform>().angle = 0;
                steer(entity, 0);
            }
        }
        //entity->getComponent<CTransform>().pos = Vec2(gridToMidPixel(path.front().x,path.front().y,entity));
       // path.erase(path.begin());
    }
}

void AIPlayroom::sVisionCone()
{
    PlayerScanner();
    EnemyScanner();
    ItemScanner();
    WallChecker();

    drawWallCheckerRays();
}

void AIPlayroom::ItemScanner()
{

    auto& vision = playerPtr->agent->getComponent<CVision>();
    auto& transform = playerPtr->agent->getComponent<CTransform>();
    Vec2  eye = transform.pos;

    // Reset visibility
    vision.seesAmmo = vision.seesFood = vision.seesCoin = false;
 

    // Build look direction
    float angRad = transform.angle * (std::numbers::pi / 180.0f);
    Vec2  lookDir{ std::cos(angRad), std::sin(angRad) };
  
    for (auto& item : items)
    {
        Vec2 P = item->entity->getComponent<CTransform>().pos;

        //FOV + range test
        if (!vision.IsTargetInFOV(eye, lookDir, P))
        {
           // vision.Item = nullptr;
            continue;


        }
        //Occlusion test
        if (!LineOfSight(eye, P))
        {

           // vision.Item = nullptr;
            continue;
        }

        //Item Seen
        switch (item->type)
        {
        case Item::ITM_AMMO:
            vision.seesAmmo = true;
            playerPtr->hasSeenAmmo = true;
            vision.Item = std::shared_ptr<Item>(item);
            break;
        case Item::ITM_HEALTH:
            vision.seesFood = true;
            playerPtr->hasSeenFood = true;
            vision.Item = std::shared_ptr<Item>(item);
            break;
        case Item::ITM_COIN:
            vision.seesCoin = true;
            playerPtr->hasSeenCoin = true;
            vision.Item = std::shared_ptr<Item>(item);
            break;
        }
        //vision.Item = item->entity;
        // Optionally set a generic Target pointer if you want to pick one
      

        // If you only care about the *first* visible item, you can break here:
         //break;
    }

}


void AIPlayroom::WallChecker()
{
    auto& vision = playerPtr->agent->getComponent<CVision>();
    auto& transform = playerPtr->agent->getComponent<CTransform>();
    Vec2  eye = transform.pos;

    // Reset visibility
   // vision.seesWall= false;

    vision.seesWall = false;
    vision.NearestBrick = nullptr;
    

    // Build look direction
    float angRad = transform.angle * (std::numbers::pi / 180.0f);
    Vec2  lookDir{ std::cos(angRad), std::sin(angRad) };

    for (auto& brick : m_entityManager.getEntities("Brick"))
    {
        Vec2 P = brick->getComponent<CTransform>().pos;

        //FOV + range test
        if (vision.IsTargetInFOV(eye, lookDir, P))
        {
            // vision.Item = nullptr;
            vision.seesWall = true;
            //Add to Bricks
            vision.LastKnownBrick = brick;
            vision.visibleBricks.push_back(brick);
            std::cout << "Wall at "<< positionToGridCordinates(brick).x << " , " << positionToGridCordinates(brick).y << std::endl;
            UpdateNearesBrick();

        }
        if (!vision.IsTargetInFOV(eye, lookDir, P))
        {
            vision.visibleBricks.erase(
                std::remove_if(vision.visibleBricks.begin(), vision.visibleBricks.end(),
                    [&](auto const& up) { return up == brick; }),
                vision.visibleBricks.end()
            );
            UpdateNearesBrick();
        }
        if (vision.visibleBricks.empty())
        {
            vision.seesWall = false;
            vision.NearestBrick = nullptr;
        }
       
    }

}





void AIPlayroom::EnemyScanner()
{
    auto& vision = playerPtr->agent->getComponent<CVision>();
    auto& players = m_entityManager.getEntities("player");
    auto& transform = playerPtr->agent->getComponent<CTransform>();
    Vec2  eye = transform.pos;
     if (agents.empty())
    {
           
            vision.seesPlayer = false;
            vision.Target = nullptr;
            return;
        
    }
    // if (!vision) return;
    vision.seesPlayer = false;
    vision.Target = nullptr;
    
    float angleRad = transform.angle * (std::numbers::pi / 180.0f);
    Vec2  lookDir{ std::cos(angleRad), std::sin(angleRad) };

   

    for (auto& enemy : agents) {
        Vec2 enemyPos = enemy->agent->getComponent<CTransform>().pos;

        if (!vision.IsTargetInFOV(eye, lookDir, enemyPos)) {
            continue;
        }
        if (!LineOfSight(eye, enemyPos)) {
            continue;
        }

        vision.seesPlayer = true;
        vision.Target = enemy->agent;
       

    }
}

void AIPlayroom::PlayerScanner()
{
   
    auto& players = m_entityManager.getEntities("player");
    if (agents.empty()) return;

    for (auto& enemy : agents) {
        auto& vision = enemy->agent->getComponent<CVision>();
        auto& transform = enemy->agent->getComponent<CTransform>();
        Vec2  eye = transform.pos;

        vision.seesPlayer = false;
        vision.Target = nullptr;

        // Convert degrees to radians and build forward vector
        float angleRad = transform.angle * (std::numbers::pi / 180.0f);
        Vec2  lookDir{ std::cos(angleRad), std::sin(angleRad) };

        Vec2 playerPos = playerPtr->agent->getComponent<CTransform>().pos;
        // Single FOV + range check
        if (!vision.IsTargetInFOV(eye, lookDir, playerPos)) {
            vision.seesPlayer = false;
            vision.Target = nullptr;
             continue;
        }
        if (!LineOfSight(eye, playerPos)) {
            vision.seesPlayer = false;
            vision.Target = nullptr;
            continue;
        }
        if (players.empty()) {
            vision.seesPlayer = false;
            vision.Target = nullptr;
             return;
        }

            vision.seesPlayer = true;
            vision.Target = players[0]; //Change this so that it sets whatever it sees as the target
            //std::cout << "SAW THE PLAYER\n";
    }


}

void AIPlayroom::drawVisionCone()
{
    for (auto& enemy : m_entityManager.getEntities("agent")) {
        auto& vision = enemy->getComponent<CVision>();
        auto& transform = enemy->getComponent<CTransform>();

        sf::VertexArray visionCone(sf::TrianglesFan, 12);
        visionCone[0].position = sf::Vector2f(transform.pos.x, transform.pos.y);
        visionCone[0].color = sf::Color(255, 255, 0, 100);
        if (vision.seesPlayer) { visionCone[0].color = sf::Color(255, 0, 0, 100); }

        for (int i = 0; i <= 10; i++) {
            float angle = transform.angle - vision.fovAngle * 0.5f + (vision.fovAngle / 10.0f) * i;
            float rad = angle * (std::numbers::pi / 180.0f);
            Vec2 point = transform.pos + Vec2(cos(rad), sin(rad)) * vision.visionRange;

            visionCone[i + 1].position = sf::Vector2f(point.x, point.y);
            visionCone[i + 1].color = sf::Color(255, 255, 0, 100);
            if (vision.seesPlayer) { visionCone[i + 1].color = sf::Color(255, 0, 0, 100); }
        }

        m_game->window().draw(visionCone);
    }

    for (auto& enemy : m_entityManager.getEntities("player")) {
        auto& vision = enemy->getComponent<CVision>();
        auto& transform = enemy->getComponent<CTransform>();

        sf::VertexArray visionCone(sf::TrianglesFan, 12);
        visionCone[0].position = sf::Vector2f(transform.pos.x, transform.pos.y);
        visionCone[0].color = sf::Color(255, 255, 255, 100);//sf::Color(255, 255, 0, 100);
        if (vision.seesPlayer) { visionCone[0].color = sf::Color(255, 0, 0, 100); }
        if (vision.seesAmmo) { visionCone[0].color = sf::Color(0, 0, 255, 100); }
        if (vision.seesFood) { visionCone[0].color = sf::Color(0, 255, 0, 100); }
        if (vision.seesCoin) { visionCone[0].color = sf::Color(255, 255, 0, 100); }

      


        for (int i = 0; i <= 10; i++) {
            float angle = transform.angle - vision.fovAngle * 0.5f + (vision.fovAngle / 10.0f) * i;
            float rad = angle * (std::numbers::pi / 180.0f);
            Vec2 point = transform.pos + Vec2(cos(rad), sin(rad)) * vision.visionRange;

            visionCone[i + 1].position = sf::Vector2f(point.x, point.y);
            visionCone[i + 1].color = sf::Color(255, 255, 255, 100);//sf::Color(255, 255, 0, 100);
            if (vision.seesPlayer) { visionCone[i + 1].color = sf::Color(255, 0, 0, 100); }
            if (vision.seesAmmo) { visionCone[i + 1].color = sf::Color(0, 0, 255, 100);}
            if (vision.seesFood) { visionCone[i + 1].color = sf::Color(0, 255, 0, 100); }
            if (vision.seesCoin) { visionCone[i + 1].color = sf::Color(255, 255, 0, 100); }


        }

        m_game->window().draw(visionCone);

        drawWallCheckerRays();
    }
}

void AIPlayroom::ManageInventory()
{
    
    for (const auto& item : items)
    {
        Vec2 overlap = Physics::GetOverlap(item->entity, playerPtr->agent);
        Vec2 pOverlap = Physics::GetPreviousOverlap(item->entity, playerPtr->agent);

 
        if (0 < overlap.y && -m_gridSize.x < overlap.x)
        {
            if (0 <= overlap.x && pOverlap.x <= 0)
            {
                if (item->type == Item::ITM_COIN)
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_AMMO && playerPtr->needsAmmo())
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_HEALTH && playerPtr->needsFood())
                {
                    item->AddToPlayer(this);
                    break;

                }

            }
        }
        else if (0 < overlap.x && -m_gridSize.y < overlap.y)
        {
            if (0 <= overlap.y && pOverlap.y <= 0)
            {

                if (item->type == Item::ITM_COIN)
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_AMMO && playerPtr->needsAmmo())
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_HEALTH && playerPtr->needsFood())
                {
                    item->AddToPlayer(this);
                    break;

                }
               
            }
        }
        // check if player hits the tile from the bottom
        else if (0 < overlap.x && -m_gridSize.y < overlap.y)
        {
            if (0 <= overlap.y && pOverlap.y <= 0)
            {

                if (item->type == Item::ITM_COIN)
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_AMMO && playerPtr->needsAmmo())
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_HEALTH && playerPtr->needsFood())
                {
                    item->AddToPlayer(this);
                    break;

                }
              
            }
        }
        // check player and tile side collide
        else if (0 < overlap.y && -m_gridSize.x < overlap.x)
        {
            if (0 <= overlap.x && pOverlap.x <= 0)
            {
                if (item->type == Item::ITM_COIN)
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_AMMO && playerPtr->needsAmmo())
                {
                    item->AddToPlayer(this);
                    break;
                }
                if (item->type == Item::ITM_HEALTH && playerPtr->needsFood())
                {
                    item->AddToPlayer(this);
                    break;

                }
             
            }
        }

      
    }

}

void AIPlayroom::UpdateInventoryUI()
{
    ResizeInventory();
    for (size_t i = 0; i < inventory.size(); ++i) {
       // if (inventory[i] == nullptr) {
            std::cout << "NOT NULLPTR " << i<< std::endl;
            // Update the HUD for that slot
            if (inventory[0])
            {
                std::cout << "Item 1 " <<std::endl;
                inventoryItem1 = enumToString(inventory[0]->type);
            }
            else
            {
                inventoryItem1 = "";
            }
            if (inventory[1])
            {
                std::cout << "Item 2 " << std::endl;
                inventoryItem2 = enumToString(inventory[1]->type);
            }
            else
            {
                inventoryItem2 = "";
            }
            if (inventory[2])
            {
                std::cout << "Item 3 " << std::endl;
                inventoryItem3 = enumToString(inventory[2]->type);
            }
            else
            {
                inventoryItem3 = "";
            }
            if (inventory[3])
            {
                std::cout << "Item 4 " << std::endl;
                inventoryItem4 = enumToString(inventory[3]->type);
            }
            else
            {
                inventoryItem4 = "";
            }
            if (inventory[4])
            {
                std::cout << "Item 5 " << std::endl;
                inventoryItem5 = enumToString(inventory[4]->type);
            }
            else
            {
                inventoryItem5 = "";
            }
          
          //  return; // done
       // }
    }    
}

void AIPlayroom::ResizeInventory()
{
    size_t writeIndex = 0;

    for (size_t readIndex = 0; readIndex < inventory.size(); ++readIndex) {
        if (inventory[readIndex]) {
            if (writeIndex != readIndex) {
                inventory[writeIndex] = std::move(inventory[readIndex]);
                inventory[readIndex].reset(); // optional: clear old slot
            }
            ++writeIndex;
        }
    }
    //if (removeIndex >= inventory.size()) return; // out of bounds

    //// Delete or otherwise clean up the removed item if needed:
    //delete inventory[removeIndex];
    //// Shift everything after removeIndex one slot to the left
    //for (size_t i = removeIndex; i + 1 < inventory.size(); ++i) {
    //    inventory[i] = inventory[i + 1];
    //}
    //// Clear the now-duplicate last slot
    //inventory[inventory.size() - 1] = nullptr;
}

void AIPlayroom::drawWallCheckerRays()
{
    //NOTE: The culprit turned out to be that we were feeding std::cos/std::sin a degrees instead of radians.
    auto& transform = playerPtr->agent->getComponent<CTransform>();
    auto& wallTracker = playerPtr->agent->getComponent<CWallTracker>();

    // Clear last frame’s data
    wallTracker.lastRightRayDirs.clear();
    wallTracker.lastLeftRayDirs.clear();
    wallTracker.wallRight = false;
    wallTracker.wallLeft = false;

    // 1) Compute forward from agent’s angle (DEGREES → RADIANS)
    float angleRad = transform.angle * (std::numbers::pi / 180.0f);
    Vec2  forward = { std::cos(angleRad), std::sin(angleRad) };

    // 2) Base “right” and “left” directions (unit vectors)
    Vec2 sideDirRight = { -forward.y, forward.x };   // +90° from forward
    Vec2 sideDirLeft = { forward.y, -forward.x };  // -90° from forward

    // 3) Prepare fan parameters
    float halfArcRad = (wallTracker.sideArcAngle * 0.5f) * (std::numbers::pi / 180.0f);
    int   N = wallTracker.numSideRays;
    float maxDist = wallTracker.traceDistance;

    // 4) CAST RIGHT-SIDE FAN
    {
        float baseAngleR = std::atan2(sideDirRight.y, sideDirRight.x);
        float bestDistR = maxDist + 1.0f;
        Vec2  bestNormR = { 0,0 };

        for (int i = 0; i < N; ++i) {
            float tLerp = (N == 1) ? 0.5f : (float)i / float(N - 1);
            float angleOff = (tLerp * 2.0f - 1.0f) * halfArcRad;
            float castAngle = baseAngleR + angleOff;
            Vec2 castDir = { std::cos(castAngle), std::sin(castAngle) };

            // Raycast
            RaycastHit hit = LineTrace(transform.pos, castDir, maxDist);
            if (hit.hit && hit.entity && hit.entity->tag() == "Brick") {
                float d = (hit.point - transform.pos).length();
                if (d < bestDistR) {
                    bestDistR = d;
                    bestNormR = hit.normal;
                    wallTracker.wallRight = true;
                }
            }

            wallTracker.lastRightRayDirs.push_back(castDir);
        }

        if (wallTracker.wallRight) {
            wallTracker.lastHitNormalRight = bestNormR;
        }
    }

    // 5) CAST LEFT-SIDE FAN
    {
        float baseAngleL = std::atan2(sideDirLeft.y, sideDirLeft.x);
        float bestDistL = maxDist + 1.0f;
        Vec2  bestNormL = { 0,0 };

        for (int i = 0; i < N; ++i) {
            float tLerp = (N == 1) ? 0.5f : (float)i / float(N - 1);
            float angleOff = (tLerp * 2.0f - 1.0f) * halfArcRad;
            float castAngle = baseAngleL + angleOff;
            Vec2 castDir = { std::cos(castAngle), std::sin(castAngle) };

            // Raycast
            RaycastHit hit = LineTrace(transform.pos, castDir, maxDist);
            if (hit.hit && hit.entity && hit.entity->tag() == "Brick") {
                float d = (hit.point - transform.pos).length();
                if (d < bestDistL) {
                    bestDistL = d;
                    bestNormL = hit.normal;
                    wallTracker.wallLeft = true;
                }
            }

            wallTracker.lastLeftRayDirs.push_back(castDir);
        }

        if (wallTracker.wallLeft) {
            wallTracker.lastHitNormalLeft = bestNormL;
        }
    }

    // 6) DRAW BOTH SETS OF RAYS (right in cyan, left in magenta, for instance)
    for (auto& dir : wallTracker.lastRightRayDirs) {
        Vec2 start = transform.pos;
        Vec2 end = transform.pos + dir * maxDist;
        Vec2 d = end - start;
        float len = d.length();
        float ang = std::atan2(d.y, d.x) * (180.0f / std::numbers::pi);

        sf::RectangleShape line(sf::Vector2f(len, 3.0f));
        if (wallTracker.wallRight)
        {

            line.setFillColor(sf::Color::Red);
        }
        else line.setFillColor(sf::Color::Cyan);
        line.setOrigin(0.0f, 1.5f);
        line.setPosition(start.x, start.y);
        line.setRotation(ang);
        m_game->window().draw(line);
    }

    for (auto& dir : wallTracker.lastLeftRayDirs) {
        Vec2 start = transform.pos;
        Vec2 end = transform.pos + dir * maxDist;
        Vec2 d = end - start;
        float len = d.length();
        float ang = std::atan2(d.y, d.x) * (180.0f / std::numbers::pi);

        sf::RectangleShape line(sf::Vector2f(len, 3.0f));
        if (wallTracker.wallLeft)
        {

            line.setFillColor(sf::Color::Red);
        }
        else line.setFillColor(sf::Color::Magenta);
        line.setOrigin(0.0f, 1.5f);
        line.setPosition(start.x, start.y);
        line.setRotation(ang);
        m_game->window().draw(line);
    }

}

bool AIPlayroom::RayIntersectsAABB(const Vec2& origin, const Vec2& direction, const Vec2& boxCenter, const Vec2& halfExtents, float maxDist, float& tHit, Vec2& hitNormal)
{
    // Slab method: compute intersection times for x and y slabs
    float tMin = 0.0f;
    float tMax = maxDist;

    hitNormal = Vec2(0, 0); // Default

    for (int i = 0; i < 2; ++i) { // Loop for x and y
        float rayOrig = (i == 0) ? origin.x : origin.y;
        float rayDir = (i == 0) ? direction.x : direction.y;
        float boxMin = (i == 0) ? boxCenter.x - halfExtents.x : boxCenter.y - halfExtents.y;
        float boxMax = (i == 0) ? boxCenter.x + halfExtents.x : boxCenter.y + halfExtents.y;

        if (std::abs(rayDir) < 1e-6f) {
            // Ray is parallel to the slab. Check if origin is inside.
            if (rayOrig < boxMin || rayOrig > boxMax) {
                return false; // No intersection
            }
        }
        else {
            // Compute intersection distances with slab planes
            float invDir = 1.0f / rayDir;
            float t1 = (boxMin - rayOrig) * invDir;
            float t2 = (boxMax - rayOrig) * invDir;

            if (t1 > t2) std::swap(t1, t2); // Ensure t1 <= t2

            if (t1 > tMin) {
                tMin = t1;
                hitNormal = Vec2(0, 0);
                if (i == 0) hitNormal.x = (rayDir > 0) ? -1.0f : 1.0f;
                else        hitNormal.y = (rayDir > 0) ? -1.0f : 1.0f;
            }

            if (t2 < tMax) tMax = t2;

            if (tMin > tMax || tMax < 0.0f) {
                return false; // No intersection
            }
        }
    }

    tHit = tMin;
    return (tHit >= 0.0f && tHit <= maxDist);
}

AIPlayroom::RaycastHit AIPlayroom::LineTrace(const Vec2& origin, const Vec2& direction, float maxDist)
{
    RaycastHit bestHit;
    bestHit.hit = false;
    bestHit.distance = maxDist + 1.0f;  // anything bigger than maxDist

    // Loop through all wall‐tagged entities
    for (auto& wall : m_entityManager.getEntities("Brick")) {
        // Each wall has a CTransform (position) and CBoundingBox (half‐size)
        auto& wTransform = wall->getComponent<CTransform>();
        auto& wBox = wall->getComponent<CBoundingBox>();

        float tHit;
        Vec2  hitNormal;
        bool  ok = RayIntersectsAABB(
            origin,
            direction,
            wTransform.pos,
            wBox.halfSize,
            maxDist,
            tHit,
            hitNormal
        );
        if (ok && tHit < bestHit.distance) {
            bestHit.hit = true;
            bestHit.entity = wall.get();            // raw pointer, or store shared_ptr
            bestHit.distance = tHit;
            bestHit.normal = hitNormal;
            bestHit.point = origin + direction * tHit;
        }
    }

    return bestHit;
}

void AIPlayroom::SpawnEnemies()
{
    auto e1 = m_entityManager.addEntity("agent");
    e1->addComponent<CAnimation>(m_game->assets().getAnimation("GreenAgent"), true);
    e1->addComponent<CTransform>(
        gridToMidPixel(0, 11, e1),
        Vec2(3, 0),
        Vec2(1, 1),
        0
    );
    e1->addComponent<CBoundingBox>(Vec2(64, 64));
    e1->addComponent<CVision>();

    auto e2 = m_entityManager.addEntity("agent");
    e2->addComponent<CAnimation>(m_game->assets().getAnimation("GreenAgent"), true);
    e2->addComponent<CTransform>(
        gridToMidPixel(11, 0, e2),
        Vec2(3, 0),
        Vec2(1, 1),
        0
    );
    e2->addComponent<CBoundingBox>(Vec2(64, 64));
    e2->addComponent<CVision>();


    agents.emplace_back(make_unique<GreenAgent>(e1, this));
    agents.emplace_back(make_unique<GreenAgent>(e2, this));
}



bool AIPlayroom::pointInTriangle(const Vec2& P, const Vec2& A, const Vec2& B, const Vec2& C)
{
    Vec2 v0 = C - A, v1 = B - A, v2 = P - A;
    float d00 = v0.dot(v0), d01 = v0.dot(v1);
    float d11 = v1.dot(v1), d20 = v2.dot(v0);
    float d21 = v2.dot(v1);
    float denom = d00 * d11 - d01 * d01;
    float v = (d11 * d20 - d01 * d21) / denom;
    float w = (d00 * d21 - d01 * d20) / denom;
    float u = 1.0f - v - w;
    return (u >= 0) && (v >= 0) && (w >= 0);
}

void AIPlayroom::steer(const std::shared_ptr<Entity>& entity, float targetAngle)
{
    if (entity->getComponent<CTransform>().angle==targetAngle) return;

    float turnAngle = fmod(targetAngle - entity->getComponent<CTransform>().angle + 360, 360); // Normalize difference

    if (turnAngle<180)
    {
        //clockwise
        entity->getComponent<CTransform>().angle++;
    }
    if (turnAngle > 180)
    {
        //counter clock wise
        entity->getComponent<CTransform>().angle--;
    }
    entity->getComponent<CTransform>().angle = fmod(entity->getComponent<CTransform>().angle + 360, 360);
}

void AIPlayroom::aimAndShoot(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target)
{
    bool up = false, down = false, left = false, right = false;
    //TODO: Find a cleaner a way to do this
    if (entity->getComponent<CTransform>().pos.x < target->getComponent<CTransform>().pos.x)
    {
        //move Left
        left = false;
        right = true;
    }if (entity->getComponent<CTransform>().pos.x > target->getComponent<CTransform>().pos.x)
    {
        //move Right
        left = true;
        right = false;
    }
    if (entity->getComponent<CTransform>().pos.y < target->getComponent<CTransform>().pos.y)
    {
        //move Down
        down = true;
        up = false;

    }if (entity->getComponent<CTransform>().pos.y > target->getComponent<CTransform>().pos.y)
    {
        //move Up
        up = true;
        down = false;
    }
    //0=right,90=down,180 =left, 270=up

    if (up && left)
    {
        //entity->getComponent<CTransform>().angle = 225;
        steer(entity, 225);
    }
    if (up && right)
    {
        //entity->getComponent<CTransform>().angle = 315;
        steer(entity, 315);
    }
    if (down && left)
    {
        // entity->getComponent<CTransform>().angle = 135;
        steer(entity, 135);
    }
    if (down && right)
    {
        // entity->getComponent<CTransform>().angle = 45;
        steer(entity, 45);
    }
    if (up)
    {
        //entity->getComponent<CTransform>().angle = 270;
        steer(entity, 270);
    }
    if (down)
    {
        //entity->getComponent<CTransform>().angle = 90;
        steer(entity, 90);
    }
    if (left)
    {
        // entity->getComponent<CTransform>().angle = 180;
        steer(entity, 180);
    }
    if (right)
    {
        // entity->getComponent<CTransform>().angle = 0;
        steer(entity, 0);
    }
}

float AIPlayroom::GetTurnAngle(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& Target)
{
    //Turn towards Target
    float deltaX = Target->getComponent<CTransform>().pos.x - entity->getComponent<CTransform>().pos.x;
    float deltaY = Target->getComponent<CTransform>().pos.y - entity->getComponent<CTransform>().pos.y;
    float angleRadians = std::atan2(deltaY, deltaX); // Angle in radians
    float angleDegrees = angleRadians * (180.0f / std::numbers::pi); // Convert to degrees if needed
    return angleDegrees;
}

float AIPlayroom::GetTurnAngle(const Vec2& entity, const Vec2& Target)
{
    float deltaX = Target.x - entity.x;
    float deltaY = Target.y - entity.y;
    float angleRadians = std::atan2(deltaY, deltaX); // Angle in radians
    float angleDegrees = angleRadians * (180.0f / std::numbers::pi); // Convert to degrees if needed
    return angleDegrees;;
}

void AIPlayroom::TurnTowardsTarget(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity> Target, int randDev)
{  
    if(Target)
    {
    //Retrieve positions
    const Vec2& shooterPos = entity->getComponent<CTransform>().pos;
    const Vec2& targetPos = Target->getComponent<CTransform>().pos;

    //Compute angle in radians
    float deltaX = targetPos.x - shooterPos.x;
    float deltaY = targetPos.y - shooterPos.y;
    float angleRadians = std::atan2(deltaY, deltaX);  // correct quadrant :contentReference[oaicite:2]{index=2}

    //Convert to degrees
    float angleDegrees = angleRadians * (180.0f / std::numbers::pi);

    //Assign to the transform’s angle
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> dis(-randDev, randDev);  // Uniform distribution in the range [min, max]

    entity->getComponent<CTransform>().angle = angleDegrees + dis(gen);
    }
}

void AIPlayroom::TurnTowardsPosition(const std::shared_ptr<Entity>& entity, const Vec2& Position, int randDev)
{
    
        //Retrieve positions
        const Vec2& shooterPos = entity->getComponent<CTransform>().pos;
        const Vec2& targetPos = Position;

        //Compute angle in radians
        float deltaX = targetPos.x - shooterPos.x;
        float deltaY = targetPos.y - shooterPos.y;
        float angleRadians = std::atan2(deltaY, deltaX);  // correct quadrant :contentReference[oaicite:2]{index=2}

        //Convert to degrees
        float angleDegrees = angleRadians * (180.0f / std::numbers::pi);

        //Assign to the transform’s angle
        std::random_device rd;
        std::mt19937 gen(rd());  // Mersenne Twister engine
        std::uniform_int_distribution<> dis(-randDev, randDev);  // Uniform distribution in the range [min, max]

        entity->getComponent<CTransform>().angle = angleDegrees + dis(gen);
    
}

void AIPlayroom::RunBehaviourTrees()
{
    for (auto& agent : agents) {
        if(agent)
            agent->update();
    }
}

Vec2 AIPlayroom::GetOppositeDirection(Vec2 targetPosition, Vec2 playerPosition)
{

    int dx=0;
    int dy=0;
    if (targetPosition.x > playerPosition.x)
    {
        dx = -1;
    }
    else if (targetPosition.x < playerPosition.x)
    {
        dx = 1;
    }
    if (targetPosition.y > playerPosition.y)
    {
        dy = -1;
    }
    else if (targetPosition.y < playerPosition.y)
    {
        dy = 1;
    }
    Vec2 oppositeDirection(dx, dy);
    return oppositeDirection;
}

Vec2 AIPlayroom::GetSafeSpot(Vec2 oppositeDirection, Vec2  playePosition)
{    
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> disx(5, 10);  // Uniform distribution in the range [min, max]
    std::uniform_int_distribution<> disy(5, 10);  // Uniform distribution in the range [min, max]
    //multiply them by direction
    int xDir = disx(gen)* oppositeDirection.x;
    int yDir = disy(gen)* oppositeDirection.y;
    //Get Safe Spot
    Vec2 safeSpot{ playePosition.x+xDir,playePosition.y+yDir};
    //Clap Safe spot
    safeSpot = {
    std::clamp(safeSpot.x, 0.0f, static_cast<float>(navmesh.NAVMESH_WIDTH - 1)),
    std::clamp(safeSpot.y, 0.0f, static_cast<float>(navmesh.NAVMESH_HEIGHT - 1))
    };
    //check if it's walkable
    if (navmesh.navMesh[safeSpot.x][safeSpot.y].walkable)
    {
        std::cout << "Found a safe spot" << std::endl;
        return safeSpot;
    }
    else
    {
        std::cout << "Can't walk to safe spot, trying again" << std::endl;
        GetSafeSpot(oppositeDirection, playePosition);
    }
}

Vec2 AIPlayroom::GetRandomWanderSpot()
{
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> disx(0, navmesh.NAVMESH_WIDTH - 1);  // Uniform distribution in the range [min, max]
    std::uniform_int_distribution<> disy(0, navmesh.NAVMESH_HEIGHT - 1);  // Uniform distribution in the range [min, max]
    //multiply them by direction
    int xVal = disx(gen);
    int yVal = disy(gen);
    //Get Safe Spot
    Vec2 safeSpot{ static_cast<float>(xVal),static_cast<float>(yVal) };
    //check if it's walkable
    if (navmesh.navMesh[safeSpot.x][safeSpot.y].walkable)
    {
        std::cout << "Found a random spot "<< safeSpot.x<< " , "<< safeSpot.y << std::endl;
        return safeSpot;
    }
    else
    {
        std::cout << "Can't walk to random spot, trying again" << std::endl;
        GetRandomWanderSpot();
    }
}

void AIPlayroom::CreateEntity(std::string tag, Vec2 position, std::string AnimationName)
{
    auto entity = m_entityManager.addEntity(tag);
    entity->addComponent<CAnimation>(m_game->assets().getAnimation(AnimationName), true);
    entity->addComponent<CTransform>(
        gridToMidPixel(position.x,position.y, entity),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    entity->addComponent<CBoundingBox>(Vec2(64, 64));
}

void AIPlayroom::UpdateNearesBrick()
{
    auto& vision = playerPtr->agent->getComponent<CVision>();
    auto& transform = playerPtr->agent->getComponent<CTransform>();

    vision.NearestBrick = nullptr;
    float minDist = std::numeric_limits<float>::max();

    // Iterate through all visible bricks
    for (const auto& brick : vision.visibleBricks)
    {
        if (!brick) continue; // Safety check

        auto& brickTransform = brick->getComponent<CTransform>();

        float distSq = (brickTransform.pos - transform.pos).lengthSq(); // Squared distance for efficiency
        if (distSq < minDist)
        {
            minDist = distSq;
            vision.NearestBrick = brick;
        }
    }
}


void AIPlayroom::sLifespan() {
    // Check lifespan of entities that have them, and destroy them if they go over
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->hasComponent<CLifespan>()) {
            if (entity->getComponent<CAnimation>().animation.getName() == "CoinSpin") {
                if (entity->getComponent<CAnimation>().animation.hasEnded()) {
                    entity->destroy();
                }
            }
            else {
                auto& eLife = entity->getComponent<CLifespan>();
                if (m_currentFrame - eLife.frameCreated >= eLife.lifespan) {
                    entity->destroy();
                }
            }
        }
    }

    // control bullet quantity, be spawned every 10 frames
    for (const auto& entity : m_entityManager.getEntities("bullet")) {
        auto& bulletLife = entity->getComponent<CLifespan>();
        if (m_currentFrame - bulletLife.frameCreated == 20) {
           // AIAgent->getComponent<CInput>().canShoot = true;
        }
    }
}

void AIPlayroom::sMovement()
{
    float bulletSpeed=2;
    // update all entities positions
  for (const auto& entity : m_entityManager.getEntities("bullet")) {
    auto& transform = entity->getComponent<CTransform>();
    transform.prevPos = transform.pos;
    transform.pos.x += transform.velocity.x* bulletSpeed;
    transform.pos.y += transform.velocity.y* bulletSpeed;
}
}

void AIPlayroom::sCollision() {
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // Implement Physics::GetOverlap() function, use it inside this function

    // Implement bullet/tile collisions
    // Destroy the tile if it has a Brick animation

    for (const auto& bullet : m_entityManager.getEntities("bullet")) {
        //NOTE: .lock() is used to get a shared pointer from a weak pointer
        if (auto inst = bullet->Instigator.lock()) {
            // inst is shared_ptr<Entity> of shooter
            for (const auto& agent : agents) {
                // ai->agent is shared_ptr<Entity> in BaseAIAgent
                if (inst.get() != agent->agent.get()) {
                    //auto& gridSize = agent->agent->getComponent<CBoundingBox>().size;
                    // This bullet wasn’t fired by this AI
                    Vec2 overlap = Physics::GetOverlap(bullet, agent->agent);
                    Vec2 pOverlap = Physics::GetPreviousOverlap(bullet, agent->agent);
                    
                  
                        if (0 < overlap.y && -m_gridSize.x < overlap.x)
                        {
                                if (0 <= overlap.x && pOverlap.x <= 0) 
                                {
                                    spawnBrickDebris(bullet);
                                    agent->TakeDamage(bullet->Instigator.lock());
                                   // bullet->destroy();
                                }
                        }
                        if (0 < overlap.x && -m_gridSize.y < overlap.y)
                        {
                            if (0 <= overlap.y && pOverlap.y <= 0) 
                            {
                               spawnBrickDebris(bullet);
                               agent->TakeDamage(bullet->Instigator.lock());
                               // bullet->destroy();
                            }
                        }
                    // check if player hits the tile from the bottom
                         if (0 < overlap.x && -m_gridSize.y < overlap.y )
                        {
                            if (0 <= overlap.y && pOverlap.y <= 0) 
                            {
                                spawnBrickDebris(bullet);
                                agent->TakeDamage(bullet->Instigator.lock());
                               // bullet->destroy();
                            }
                        }
                    // check player and tile side collide
                         if (0 < overlap.y && -m_gridSize.x < overlap.x)
                        {
                            if (0 <= overlap.x && pOverlap.x <= 0) 
                            {
                                spawnBrickDebris(bullet);
                                agent->TakeDamage(bullet->Instigator.lock());
                               // bullet->destroy();
                            }
                        }


                }
            }
        }
    }



    for (const auto& bullet : m_entityManager.getEntities("bullet")) {
        for (const auto& tile : m_entityManager.getEntities("tile")) {
            // check bullet and tile side collide
            Vec2 overlap = Physics::GetOverlap(bullet, tile);
            Vec2 pOverlap = Physics::GetPreviousOverlap(bullet, tile);
            if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                if (0 <= overlap.x && pOverlap.x <= 0) {
                    if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                        spawnBrickDebris(tile);
                    }
                    bullet->destroy();
                }
            }
        }
    }

    ManageInventory();
}

void AIPlayroom::sAnimation()
{
    if (playerPtr->agent->getComponent<CAnimation>().animation.hasEnded()&& playerPtr->agent->getComponent<CAnimation>().animation.getName()=="Healing")
    {
        playerPtr->agent->getComponent<CAnimation>().animation = m_game->assets().getAnimation("BlueAgent");
    }
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->getComponent<CAnimation>().animation.hasEnded() && !entity->getComponent<CAnimation>().repeat) {
            entity->destroy();
        }
        if (entity->hasComponent<CAnimation>()) {
            entity->getComponent<CAnimation>().animation.update();
        }
    }
}

void AIPlayroom::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "MoveAgent") { playerPtr->health=22; }

      
    }
 
}


void AIPlayroom::onEnd() {
    // when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void AIPlayroom::sRender() {
    // color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window().clear(sf::Color(54, 54, 54));
    }
    else {
        m_game->window().clear(sf::Color(4, 4, 4));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
   // set the viewport of the window to be centered on the player if it's far enough right
    auto& pPos = playerPtr->agent->getComponent<CTransform>().pos;
    float windowCenterX = std::max(float(m_game->window().getSize().x) / 2.0f, pPos.x);
    float windowCenterY = std::max(float(m_game->window().getSize().y) / 2.0f, pPos.y);
    sf::View view = m_game->window().getView();
    view.setCenter(pPos.x, pPos.y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto& e : m_entityManager.getEntities()) {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (const auto& e : m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    if(m_drawGrid)
        navmesh.DrawPath(m_game->window());
    drawVisionCone();
    //// draw the grid 
    //if (m_drawGrid) {
    //    float leftX = float(m_game->window().getView().getCenter().x) - width() / 2.0f;
    //    float rightX = leftX + width() + m_gridSize.x;
    //    float nextGridX = leftX - float((int)leftX % (int)m_gridSize.x);

    //    for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
    //        drawLine(Vec2(x, 0), Vec2(x, height()));
    //    }

    //    for (float y = 0; y < height(); y += float(m_gridSize.y)) {
    //        drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

    //        for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
    //            std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
    //            std::string yCell = std::to_string(((int)y / (int)m_gridSize.y));
    //            m_gridText.setString("(" + xCell + "," + yCell + ")");
    //            m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
    //            m_game->window().draw(m_gridText);
    //        }
    //    }
    //}

    if (m_drawGrid) {
    float leftX = 0.0f;
    float rightX = navmesh.NAVMESH_WIDTH * m_gridSize.x;
    float bottomY = 0.0f;
    float topY = navmesh.NAVMESH_HEIGHT * m_gridSize.y;

    // Vertical lines (X varies, Y spans full navmesh height)
    for (float x = 0.0f; x <= rightX; x += m_gridSize.x) {
        drawLine(
            Vec2(x, height() - 0.0f),                      // Bottom-left Y flipped
            Vec2(x, height() - topY),                  // Top-left Y flipped
            sf::Color::Green
        );
    }
    // Horizontal grid lines (flip Y to draw from bottom-left)
    for (float y = 0.0f; y <= topY; y += m_gridSize.y) {
        float flippedY = height() - y;
        drawLine(Vec2(leftX, flippedY), Vec2(rightX, flippedY), sf::Color::Green);

        // Cell labels (flip Y)
        for (float x = leftX; x <= rightX; x += m_gridSize.x) {
            int xCell = static_cast<int>(x / m_gridSize.x);
            int yCell = static_cast<int>(y / m_gridSize.y);

            m_gridText.setString("(" + std::to_string(xCell) + "," + std::to_string(yCell) + ")");
            m_gridText.setPosition(x + 3, flippedY - m_gridSize.y + 3);  // Position inside the cell
            m_gridText.setFillColor(sf::Color::Green);
            m_game->window().draw(m_gridText);
        }
    }
}
    int currentHealth = playerPtr->getHealth();
   
  
    sf::Text HUD("health : " + std::to_string(currentHealth)+
        "\nitems : [" + inventoryItem1 + "] [" + inventoryItem2 + "] [" + inventoryItem3 + "] [" + inventoryItem4 +  "] [" + inventoryItem5 + "]"+
        "\nbehavior : "+ behaviourName +
        "\nstatus : " + behaviourSTatus +
        "\nSCORE : " + std::to_string(PlayerScore)
        , m_game->assets().getFont("Mario"), 20);
    HUD.setFillColor(sf::Color::White);
    HUD.setPosition(
        pPos.x-625, pPos.y-375
    );
    m_game->window().draw(HUD);
}

void AIPlayroom::changePlayerStateTo(const std::string& state) {
  /*  auto& prev = AIAgent->getComponent<CState>().previousState;
    if (prev != state) {
        prev = AIAgent->getComponent<CState>().state;
        AIAgent->getComponent<CState>().state = state;
        AIAgent->getComponent<CState>().changeAnimation = true;
    }
    else {
        AIAgent->getComponent<CState>().changeAnimation = false;
    }*/
}

void AIPlayroom::spawnBrickDebris(const std::shared_ptr<Entity>& tile) {
    auto debris = m_entityManager.addEntity("debris");
    debris->addComponent<CAnimation>(m_game->assets().getAnimation("Explosion"), false);

    debris->addComponent<CTransform>(tile->getComponent<CTransform>());
    tile->destroy();
    // tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("BrickDebris");
    //tile->addComponent<CLifespan>(10, m_currentFrame);
    debris->getComponent<CLifespan>().lifespan = 10;
    debris->getComponent<CLifespan>().frameCreated = m_currentFrame;

    
}

void AIPlayroom::spawnCoinSpin(const std::shared_ptr<Entity>& tile) {
    auto coin = m_entityManager.addEntity("coinspin");
    coin->addComponent<CAnimation>(
        m_game->assets().getAnimation("CoinSpin"),
        false
    );
    coin->addComponent<CTransform>(
        Vec2(
            tile->getComponent<CTransform>().pos.x,
            tile->getComponent<CTransform>().pos.y - m_gridSize.y
        ),
        Vec2(0, 0),
        tile->getComponent<CTransform>().scale,
        0
    );
    coin->addComponent<CLifespan>(30, m_currentFrame);
}
