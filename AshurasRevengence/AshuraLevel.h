#pragma once
#include "../Action.h"
#include "../Entity.h"
#include "../EntityManager.h"
#include "../Scene.h"
#include "WFC/WFC.h"
//WFC includes
#include <unordered_map>
#include <map>
#include <iostream>
#include <array>
#include "../Quadtrees/Quadtree.h"
class AshuraLevel :public Scene
{
    struct PlayerConfig {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = { 64, 64 };
    sf::Text m_gridText;

    void init(const std::string& levelPath);

    Vec2 gridToMidPixel(float, float, const std::shared_ptr<Entity>& entity);

    void loadLevel(const std::string& fileName);

    void spawnPlayer();
    void SpawnEnemies();
    int generateRandomNumber(int min, int max);
    void spawnBullet(const std::shared_ptr<Entity>& entity);

    void sMovement();

    void sLifespan();

    void sCollision();

    void sAnimation();

    void sRender() override;

    void sDoAction(const Action& action) override;

    void onEnd() override;

    void changePlayerStateTo(const std::string& state);

    void spawnCoinSpin(const std::shared_ptr<Entity>& tile);

    void spawnBrickDebris(const std::shared_ptr<Entity>& tile);

public:
    friend class WFC;
    WFC* wfcInstance;
    AshuraLevel(GameEngine* gameEngine, const std::string& levelPath);
    Quadtree* quadtree=nullptr;
    void update() override;
    bool q_drawQuadtrees = false;
    std::vector<Entity*> bullets;
    std::vector<Entity*> enemies;
    sf::FloatRect getGlobalBounds(Entity* entity);
};

