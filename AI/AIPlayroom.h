#pragma once
#include "../Action.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../Scene.h"
#include "Pathfinding/AStar.h"
#include "Agents/BaseAIAgent.h"
#include "Agents/BlueAgent.h"
#include "Item.h"
#include "../AI/House/House.h"
#include <vector>
class House::Room;
class GreenAgent;
class Item;
class AIPlayroom :public Scene {

    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

    struct RaycastHit {
        bool    hit;             // true if hit something
        Vec2    point;           // world‐space hit point
        Vec2    normal;          // surface normal at the hit
        Entity* entity;          // pointer to whatever you hit (nullptr if none)
        float   distance;   // distance from ray origin to hit point
    };

protected:
   // std::shared_ptr<Entity> AIAgent;
    //std::shared_ptr<Entity> AIAgent3;
    std::string m_levelPath;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = { 64, 64 };
    sf::Text m_gridText;
    BlueAgent* playerPtr = nullptr;
    void init(const std::string& levelPath);

    void sLifespan();
    void sMovement();

    void sCollision();

    void sAnimation();

    void sRender() override;

    void sDoAction(const Action& action) override;

    void onEnd() override;


    void spawnCoinSpin(const std::shared_ptr<Entity>& tile);

    void spawnBrickDebris(const std::shared_ptr<Entity>& tile);

public:
    std::stack<House::Room*> roomStack;
    std::shared_ptr<House::Room> currentRoom = nullptr;
    int ammoCount = 0;
    std::string behaviourName = "None";
    std::string behaviourSTatus = "None";
    int PlayerScore = 0;
    void RemoveAgent(BaseAIAgent* ptr);
    void RemoveItem(Item* ptr);
    void spawnBullet(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target);
    AIPlayroom(GameEngine* gameEngine, const std::string& levelPath);
    Vec2 gridToMidPixel(float, float, const std::shared_ptr<Entity>&);
    void update() override;
    //AI
    AStar navmesh;
    bool isScanningWalls = true;
    bool LineOfSight(const Vec2& A, const Vec2& P);
    bool liangBarsky(float x0, float y0, float x1, float y1,
        float xmin, float ymin, float xmax, float ymax);
    std::vector<std::unique_ptr<BaseAIAgent>> agents;
    std::vector<std::shared_ptr<Item>> items;
    std::vector<Vec2> path;
    const size_t INVENTORY_SIZE = 5;
    std::array<std::shared_ptr<Item>, 5> inventory;
    Vec2 positionToGridCordinates(const std::shared_ptr<Entity>& entity);
    Vec2 positionToGridCordinates(const Vec2& cordinates);
    void MoveEntity(const std::shared_ptr<Entity>& entity, std::vector<Vec2>& path);
    void sVisionCone();
    void ItemScanner();
    void WallChecker();
    void EnemyScanner();
    void PlayerScanner();
    void drawVisionCone();
    void ManageInventory();
    void UpdateInventoryUI();
    void ResizeInventory();
    void drawWallCheckerRays();
    bool RayIntersectsAABB(
        const Vec2& origin,           // Ray start point
        const Vec2& direction,        // Normalized ray direction
        const Vec2& boxCenter,        // Center of the box
        const Vec2& halfExtents,      // Half-size of the box
        float maxDist,                // Max distance for the ray
        float& tHit,                  // Out: distance along the ray to the hit point
        Vec2& hitNormal               // Out: normal of the box face that was hit
    );
    // Additional helper to check if a tile is a wall
    bool isWallAt(const Vec2& tile);
    RaycastHit LineTrace(const Vec2& origin, const Vec2& direction, float maxDist);
    std::map<int, std::string> itemToString = {
     { 0, "NONE" },
     { 1, "Health" },
     { 2, "Ammo" },
     { 3, "Coin" },

    };
    std::string enumToString(int item)
    {
        std::cout << "enum to String " << itemToString[item] << std::endl;
        return itemToString[item];
    }
    std::string inventoryItem1, inventoryItem2, inventoryItem3, inventoryItem4, inventoryItem5 = " ";

    void SpawnEnemies();
    bool pointInTriangle(const Vec2& P, const Vec2& A,
        const Vec2& B, const Vec2& C);
    void steer(const std::shared_ptr<Entity>& entity, float targetAngle);
    void aimAndShoot(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target);
    float GetTurnAngle(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& Target);
    float GetTurnAngle(const Vec2& entity, const Vec2& Target);
    void TurnTowardsTarget(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity> Target, int randDev=0);
    void TurnTowardsPosition(const std::shared_ptr<Entity>& entity, const Vec2& Position, int randDev = 0);
    void RunBehaviourTrees();
    Vec2 GetOppositeDirection(Vec2 targetPosition, Vec2 PlayerPosition);
    Vec2 GetSafeSpot(Vec2 oppositeDirection, Vec2  playePosition);
    Vec2 GetRandomWanderSpot();
    void CreateEntity(std::string tag, Vec2 position, std::string AnimationName,int houseID);
    void UpdateNearesBrick();

    friend class BaseAIAgent;
};