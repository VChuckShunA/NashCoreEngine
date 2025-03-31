#pragma once
#include "../Action.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../Scene.h"
#include "Pathfinding/AStar.h"
class AIPlayroom :public Scene {

    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> AIAgent;
    std::string m_levelPath;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = { 64, 64 };
    sf::Text m_gridText;

    void init(const std::string& levelPath);

    Vec2 gridToMidPixel(float, float, const std::shared_ptr<Entity>&);

   

    void spawnBullet(const std::shared_ptr<Entity>& entity);

  

    void sLifespan();

    void sCollision();



    void sRender() override;

    void sDoAction(const Action& action) override;

    void onEnd() override;

    void changePlayerStateTo(const std::string& state);

    void spawnCoinSpin(const std::shared_ptr<Entity>& tile);

    void spawnBrickDebris(const std::shared_ptr<Entity>& tile);

public:
    AIPlayroom(GameEngine* gameEngine, const std::string& levelPath);

    void update() override;
    //AI
    AStar navmesh;
    std::vector<Vec2> path;
    Vec2 positionToGridCordinates(const std::shared_ptr<Entity>& entity);
    void MoveEntity(const std::shared_ptr<Entity>& entity, std::vector<Vec2>& path);
    void sVisionCone();
    void drawVisionCone();
    void steer(const std::shared_ptr<Entity>& entity, float targetAngle);
};

