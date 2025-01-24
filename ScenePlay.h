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

    std::unordered_map<TileType, std::unordered_map<std::string, std::vector<TileType>>> adjacencyRules = {
     {BRIDGE, {
        {"down", {T, TRACK}},
        {"up", {TRACK, TURN,CONNECTION,SKEW,DSKEW}},
        {"left", { WIRE}},
        {"right", { WIRE}}
    }},
    {COMPONENT, {
        {"down", {COMPONENT,BRIDGE, CONNECTION,SUBSTRATE,T,TRACK,TRANSITION}},
        {"up", {COMPONENT,CONNECTION,SUBSTRATE,T,TRACK,TRANSITION}},
        {"left", {COMPONENT,BRIDGE,WIRE,SUBSTRATE,T,VIAD}},
        {"right", {COMPONENT,BRIDGE,WIRE,SUBSTRATE,T,VIAD}}
    }},
    {CONNECTION, {
        {"down", {T, TRACK,TRANSITION,BRIDGE}},
        {"up", {COMPONENT}},
        {"left", {SUBSTRATE}},
        {"right", {SUBSTRATE, CORNER}}
    }},
    {CORNER, {
        {"down", {SUBSTRATE,SKEW,TURN,VIAD,VIAS,WIRE}},
        {"up", {SUBSTRATE}},
        {"left", {SUBSTRATE,TURN,CONNECTION}},
        {"right", {SUBSTRATE,TURN,VIAD,VIAS}}
    }},
    {DSKEW, {
        {"down", { BRIDGE,T,TRACK,TRANSITION}},
        {"up", {BRIDGE,TRACK,TURN,VIAS}},
        {"left", {T,TURN,SKEW}},
        {"right", {T}}
    }},
    {SKEW, {
        {"down", {DSKEW,T,TRACK,TRANSITION}},
        {"up", {CORNER,SUBSTRATE,T,WIRE}},
        {"left", {COMPONENT,SUBSTRATE,TRACK,TRANSITION}},
        {"right", {T,VIAD}}
    }},
    {SUBSTRATE, {
        {"down", {SUBSTRATE,SKEW,TURN,WIRE}},
        {"up", {SUBSTRATE,CORNER,T,WIRE}},
        {"left", {SUBSTRATE,CORNER}},
        {"right", {SUBSTRATE,TRACK,TURN}}
    }},
    {T, {
        {"down", {SKEW,SUBSTRATE,TURN,VIAD,VIAS,WIRE}},
        {"up", {TRACK,TURN,BRIDGE,SKEW,DSKEW,VIAS}},
        {"left", {SKEW,DSKEW,TURN,VIAD}},
        {"right", {DSKEW,VIAD}}
    }},
    {TRACK, {
        {"down", {TRACK, BRIDGE,DSKEW,TRANSITION}},
        {"up", {TRACK, BRIDGE,SKEW,DSKEW,TURN,CONNECTION,BRIDGE,VIAS}},
        {"left", {TRACK,SUBSTRATE,CORNER}},
        {"right", {TRACK,SUBSTRATE,TRACK,TURN}}
    }},
    {TRANSITION, {
        {"down", {BRIDGE,DSKEW,T,TRACK}},
        {"up", {BRIDGE,DSKEW,TURN,TRACK}},
        {"left", {SUBSTRATE,TRACK,CORNER}},
        {"right", {SUBSTRATE,SKEW,TURN}}
    }},
    {TURN, {
        {"down", {BRIDGE,DSKEW,T,TRACK,TRANSITION}},
        {"up", {CORNER,SUBSTRATE,T,WIRE}},
        {"left", {TRACK,SUBSTRATE,CORNER}},
        {"right", {DSKEW,VIAD,T}}
    }},
    {VIAD, {
        {"down", {WIRE,TURN,SUBSTRATE,SKEW}},
        {"up", {WIRE,SUBSTRATE,CORNER}},
        {"left", {TURN,T,SKEW,DSKEW,VIAD,WIRE}},
        {"right", {DSKEW,BRIDGE,T,VIAD,WIRE}}
    }},
    {VIAS, {
        {"down", {TRACK,TRANSITION,T,DSKEW,BRIDGE}},
        {"up", {CORNER,SUBSTRATE,T,WIRE}},
        {"left", {TRACK,SUBSTRATE,CORNER}},
        {"right", {TURN,TRACK,SUBSTRATE,SKEW}}
    }},
    {WIRE, {
        {"down", {SUBSTRATE,SKEW,TURN}},
        {"up", {CORNER,SUBSTRATE,T}},
        {"left", {WIRE,BRIDGE,SKEW,DSKEW,T,TURN,VIAD}},
        {"right", {WIRE,BRIDGE,DSKEW,T,VIAD}}
    }}
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