#pragma once

#include "Action.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"
//WFC includes
#include <unordered_map>
#include <map>
#include <iostream>
class Scene_Play : public Scene {
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

    Vec2 gridToMidPixel(float, float, const std::shared_ptr<Entity>&);

    void loadLevel(const std::string& fileName);

    void spawnPlayer();

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
    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

    void update() override;

    struct TileState {
        std::vector<int> possibleTiles; // IDs of possible tiles
        int currentTile = NULL;
        bool collapsed = false;        // Whether this cell is collapsed
    }grid[20][12];

    enum TileType {
        BRIDGE, COMPONENT, CONNECTION, CORNER, DSKEW, SKEW, SUBSTRATE, T, TRACK, TRANSITION, TURN, VIAD, VIAS, WIRE
    }tiles;

    struct TileSockets {
        int up[3];
        int down[3];
        int left[3];
        int right[3];
    };

    std::map<int, std::string> circuitToString = { 
        { BRIDGE, "BRIDGE" },
        { COMPONENT, "COMPONENT" },
        { CONNECTION, "CONNECTION" },
        { CORNER, "CORNER" },
        { DSKEW, "DSKEW" },
        { SKEW, "SKEW" },
        { SUBSTRATE, "SUBSTRATE" },
        { T, "T" },
        { TRACK, "TRACK" },
        { TRANSITION, "TRANSITION" },
        { TURN, "TURN" },
        { TURN, "TURN" },
        { VIAS, "VIAS" },
        { WIRE, "WIRE" }
    };

    std::unordered_map<TileType, std::unordered_map<std::array<int, 3>, std::vector<TileType>>> adjacencyRules = {
    {BRIDGE, {
        {{1, 2, 3}, {T, TRACK}},  // Down socket
        {{3, 2, 1}, {TRACK, TURN, CONNECTION, SKEW, DSKEW}},  // Up socket
        {{2, 2, 2}, {WIRE}},  // Left socket
        {{2, 2, 2}, {WIRE}}   // Right socket
    }},
    {COMPONENT, {
        {{1, 1, 1}, {COMPONENT, BRIDGE, CONNECTION, SUBSTRATE, T, TRACK, TRANSITION}},
        {{2, 2, 2}, {COMPONENT, CONNECTION, SUBSTRATE, T, TRACK, TRANSITION}},
        {{3, 3, 3}, {COMPONENT, BRIDGE, WIRE, SUBSTRATE, T, VIAD}},
        {{4, 4, 4}, {COMPONENT, BRIDGE, WIRE, SUBSTRATE, T, VIAD}}
    }}
        // Add other tiles here...
    };


    void ImplementWFC(TileState(&grid)[20][12]);

    void UpdateRuleSet(TileState* cell, int x, int y);

    void RenderTile(int *tileID, int *randRow, int* randCol);

    void Collapse(int currentX, int currentY);
    void SpiralTraverse(TileState(&grid)[20][12]);
    bool withinBounds(int x, int y) {
        return x >= 0 && x < 20 && y >= 0 && y < 12;
    }
    void UpdateCommonElements(int x, int y, std::vector<TileType>* rulesToCompare);
    std::string enumToString(int tile)
    {
        return circuitToString[tile];
    }
};