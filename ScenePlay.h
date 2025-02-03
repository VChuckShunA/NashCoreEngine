#pragma once

#include "Action.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Scene.h"
//WFC includes
#include <unordered_map>
#include <map>
#include <iostream>
#include <array>
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


    enum TileType {
        BRIDGE, COMPONENT, CONNECTION, CORNER, DSKEW, SKEW, SUBSTRATE, T, TRACK, TRANSITION, TURN, VIAD, VIAS, WIRE
    }tiles;

    struct TileSockets {
        std::array<int, 3> up = { 0, 0, 0 };
        std::array<int, 3> down = { 0, 0, 0 };
        std::array<int, 3> left = { 0, 0, 0 };
        std::array<int, 3> right = { 0, 0, 0 };
    };


    struct TileState {
        TileSockets sockets;
        int currentTile = NULL;
        bool collapsed = false;        // Whether this cell is collapsed
    }grid[20][12];

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
        { VIAD, "VIAD"},
        { VIAS, "VIAS" },
        { WIRE, "WIRE" }
    };
    
    std::unordered_map<TileType, std::unordered_map<std::string, std::array<int, 3>>> adjacencyRules = {
        { BRIDGE, {
            { "up",   {1, 2, 1} },
            { "down", {1, 2, 1} },
            { "left", {1, 3, 1} },
            { "right",{1, 3, 1} }
        }},
        { COMPONENT, {
            { "up",   {9, 9, 9} },
            { "down", {9, 9, 9}},
            { "left", {9, 9, 9}},
            { "right",{9, 9, 9} }
        }},
        { CONNECTION, {
            { "up",   {1, 2, 1} },
            { "down", {9, 9, 9} },
            { "left", {1, 1, 9} },
            { "right",{1, 1, 9} }
        }},
        { CORNER, {
            { "up",   {1, 1, 1} },
            { "down", {9, 1, 1}},
            { "left", {1, 1, 9}},
            { "right",{1, 1, 1} }
        }},
        { DSKEW, {
            { "up",   {1, 2, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }
        }},
        { SKEW, {
            { "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 2, 1} }
        }},
        { SUBSTRATE, {
            { "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }
        }},
        { T, {
            { "up",   {1, 1, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }
        }},
        { TRACK, {
            { "up",   {1, 2, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }
        }},
        { TRANSITION, {
            { "up",   {1, 3, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }
        }},
        { TURN, {
            { "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 2, 1} }
        }},
        { VIAD, {
            { "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }
        }},
        { VIAS, {
            { "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }
        }},
        { WIRE, {
            { "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 3, 1}},
            { "right",{1, 3, 1} }
        }},

    };
    

    void ImplementWFC(TileState(&grid)[20][12]);


    void RenderTile(TileType tileID, int *randRow, int* randCol, int rotationCount=0);

    void Collapse(int currentX, int currentY);
    void SpiralTraverse(TileState(&grid)[20][12]);
    bool withinBounds(int x, int y) {
        return x >= 0 && x < 20 && y >= 0 && y < 12;
    }
    //utils
    bool matchesRules(const std::array<int, 3>& candidate, const std::array<int, 3>& toCheck);
    std::vector<TileType> selectValidTiles(const std::unordered_map<std::string, std::array<int, 3>>& rulesToCheck);
    void UpdateCommonElements(int x, int y, std::vector<TileType>* rulesToCompare);
    std::string enumToString(int tile)
    {
        return circuitToString[tile];
    }
    void RotateTileRules(std::array<int, 3>& upRules, std::array<int, 3>& downRules, std::array<int, 3>& leftRules, std::array<int, 3>& rightRules);
    std::string arrayToString(const std::array<int, 3>& arr);
    std::array<int, 3> extractRules(TileType tile, const std::string& direction);
};