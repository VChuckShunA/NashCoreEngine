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

    void update() override;

    /*
    enum TileType {
        BRIDGE, COMPONENT, CONNECTION, CORNER, DSKEW, SKEW, SUBSTRATE, T, TRACK, TRANSITION, TURN, VIAD, VIAS, WIRE,
        BRIDGE1, BRIDGE2, BRIDGE3,
        CONNECTION1, CONNECTION2, CONNECTION3,
        CORNER1, CORNER2, CORNER3,
        DSKEW1, DSKEW2, DSKEW3,
        SKEW1, SKEW2, SKEW3,
        T1, T2, T3,
        TRACK1, TRACK2, TRACK3,
        TRANSITION1, TRANSITION2, TRANSITION3,
        TURN1, TURN2, TURN3,
        VIAD1, VIAD2, VIAD3,
        VIAS1, VIAS2, VIAS3,
        WIRE1, WIRE2, WIRE3,
    }tiles;

    struct TileSockets {
        std::array<int, 3> up = { 90, 90, 90 };
        std::array<int, 3> down = { 90, 90, 90 };
        std::array<int, 3> left = { 90, 90, 90 };
        std::array<int, 3> right = { 90, 90, 90 };
    };

    struct TileInfo {
        std::string textureName;  // Stores tile name
        int rotationCount; // Stores rotation count
        std::unordered_map<std::string, std::array<int, 3>> adjacencyRules;
    };


    struct TileState {
        TileSockets sockets;
        std::vector<TileType> possibleTiles = {
        BRIDGE, COMPONENT, CONNECTION, CORNER, DSKEW, SKEW, SUBSTRATE, T, TRACK, TRANSITION, TURN, VIAD, VIAS, WIRE,
        BRIDGE1, BRIDGE2, BRIDGE3,
        CONNECTION1, CONNECTION2, CONNECTION3,
        CORNER1, CORNER2, CORNER3,
        DSKEW1, DSKEW2, DSKEW3,
        SKEW1, SKEW2, SKEW3,
        T1, T2, T3,
        TRACK1, TRACK2, TRACK3,
        TRANSITION1, TRANSITION2, TRANSITION3,
        TURN1, TURN2, TURN3,
        VIAD1, VIAD2, VIAD3,
        VIAS1, VIAS2, VIAS3,
        WIRE1, WIRE2, WIRE3,
        }; // IDs of possible tiles
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
        { WIRE, "WIRE" },
        { BRIDGE1, "BRIDGE1" }, { BRIDGE2, "BRIDGE2" },{ BRIDGE3, "BRIDGE3" },
        { CONNECTION1, "CONNECTION1" },{ CONNECTION2, "CONNECTION2" },{ CONNECTION3, "CONNECTION3" },
        { CORNER1, "CORNER1" },{ CORNER2, "CORNER2" },{ CORNER3, "CORNER3" },
        { DSKEW1, "DSKEW1" },{ DSKEW2, "DSKEW2" },{ DSKEW3, "DSKEW3" },
        { SKEW1, "SKEW1" },{ SKEW2, "SKEW2" },{ SKEW3, "SKEW3" },
        { T1, "T1" },{ T2, "T2" },{ T3, "T3" },
        { TRACK1, "TRACK1" },{ TRACK2, "TRACK2" },{ TRACK3, "TRACK3" },
        { TRANSITION1, "TRANSITION1" },{ TRANSITION2, "TRANSITION2" },{ TRANSITION3, "TRANSITION3" },
        { TURN1, "TURN1" },{ TURN2, "TURN2" }, { TURN3, "TURN3" },
        { VIAD1, "VIAD1"}, { VIAD2, "VIAD2"}, { VIAD3, "VIAD3"},
        { VIAS1, "VIAS1" }, { VIAS2, "VIAS2" }, { VIAS3, "VIAS3" },
        { WIRE1, "WIRE1" }, { WIRE2, "WIRE2" }, { WIRE3, "WIRE3" }
    };

    std::unordered_map<TileType, TileInfo> adjacencyRules = {
        { BRIDGE, {
            "Bridge",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 2, 1} },
            { "left", {1, 3, 1} },
            { "right",{1, 3, 1} }}
        }},
        { COMPONENT, {
            "Component",0,
            {{ "up",   {9, 9, 9} },
            { "down", {9, 9, 9}},
            { "left", {9, 9, 9}},
            { "right",{9, 9, 9} }}
        }},
        { CONNECTION, {
            "Connection",0,
            {{ "up",   {1, 2, 1} },
            { "down", {9, 9, 9} },
            { "left", {1, 1, 9} },
            { "right",{1, 1, 9} }}
        }},
        { CORNER, {
            "Corner",0,
            {{ "up",   {1, 1, 1} },
            { "down", {9, 1, 1}},
            { "left", {1, 1, 9}},
            { "right",{1, 1, 1} }}
        }},
        { DSKEW, {
            "DSkew",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }}
        }},
        { SKEW, {
            "Skew",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 2, 1} }}
        }},
        { SUBSTRATE, {
            "Substrate",0,
            {{ "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }}
        }},
        { T, {
            "T",0,
            {{ "up",   {1, 1, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }}
        }},
        { TRACK, {
            "Track",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }}
        }},
        { TRANSITION, {
            "Transition",0,
            {{ "up",   {1, 3, 1} },
            { "down", {1, 2, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }}
        }},
        { TURN, {
            "Turn",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 2, 1} }}
        }},
        { VIAD, {
            "ViaD",0,
            {{ "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 2, 1}},
            { "right",{1, 2, 1} }}
        }},
        { VIAS, {
            "ViaS",0,
            {{ "up",   {1, 2, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 1, 1}},
            { "right",{1, 1, 1} }}
        }},
        { WIRE, {
            "Wire",0,
            {{ "up",   {1, 1, 1} },
            { "down", {1, 1, 1}},
            { "left", {1, 3, 1}},
            { "right",{1, 3, 1} }}
        }},

    };



    void RenderTile(TileType tileID, int* randomRow, int* randomCol, int rotationCount);

    void Collapse();
    std::pair<int, int> FindLowestEntropy();
    void UpdateNeighbourRules(int currentX, int currentY);
    void UpdatePossibleTiles(int currentX, int currentY);
    void ImplementWFC(TileState(&grid)[20][12]);
    void ResetGrid();
    //utils
    std::string enumToString(int tile)
    {
        return circuitToString[tile];
    }
    void RotateTileRules(std::array<int, 3>& upRules, std::array<int, 3>& downRules, std::array<int, 3>& leftRules, std::array<int, 3>& rightRules);
    std::string arrayToString(const std::array<int, 3>& arr);
    void ProcessTiles();*/
};

