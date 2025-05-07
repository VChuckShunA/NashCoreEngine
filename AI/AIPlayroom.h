#pragma once
#include "../Action.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../Scene.h"
#include "Pathfinding/AStar.h"
#include "Agents/BaseAIAgent.h"
#include "Agents/BlueAgent.h"
#include "Item.h"
#include <vector>
class GreenAgent;
class AIPlayroom :public Scene {

    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
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

    void changePlayerStateTo(const std::string& state);

    void spawnCoinSpin(const std::shared_ptr<Entity>& tile);

    void spawnBrickDebris(const std::shared_ptr<Entity>& tile);

public:  
    std::string behaviourName = "None";
    std::string behaviourSTatus = "None";
    int PlayerScore = 0;
    void RemoveAgent(BaseAIAgent* ptr);
    void spawnBullet(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target);
    AIPlayroom(GameEngine* gameEngine, const std::string& levelPath);
    Vec2 gridToMidPixel(float, float, const std::shared_ptr<Entity>&);
    void update() override;
    //AI
    AStar navmesh;
    bool LineOfSight(const Vec2& A, const Vec2& P);
    bool liangBarsky(float x0, float y0, float x1, float y1,
        float xmin, float ymin, float xmax, float ymax);
    std::vector<std::unique_ptr<BaseAIAgent>> agents;
    std::vector<std::unique_ptr<Item>> items;
    std::vector<Vec2> path;
    Vec2 positionToGridCordinates(const std::shared_ptr<Entity>& entity);
    void MoveEntity(const std::shared_ptr<Entity>& entity, std::vector<Vec2>& path);
    void sVisionCone();
    void ItemScanner();
    void drawVisionCone();
    bool pointInTriangle(const Vec2& P, const Vec2& A,
        const Vec2& B, const Vec2& C);
    void steer(const std::shared_ptr<Entity>& entity, float targetAngle);
    void aimAndShoot(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target);
    float GetTurnAngle(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& Target);
    float GetTurnAngle(const Vec2& entity, const Vec2& Target);
    void TurnTowardsTarget(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity> Target, int randDev=0);
    void RunBehaviourTrees();
};

