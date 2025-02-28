#pragma once
#include "Tile.h"
#include "../../EntityManager.h"
#include"../../GameEngine.h"
#include "../AshuraLevel.h"
class AshuraLevel;
class WFC
{
public:
    AshuraLevel& levelInstance;
    Tile tileInstance;
    WFC(AshuraLevel& AshuralevelInstance);

    void RenderTile(Tile::TileType tileID, int* randomRow, int* randomCol);

    void Collapse();
    std::pair<int, int> FindLowestEntropy();
    void UpdateNeighbourRules(int currentX, int currentY);
    void UpdatePossibleTiles(int currentX, int currentY);
    void ImplementWFC(Tile::TileState(&grid)[20][12]);
    void ResetGrid();
    //utils
    
    void RotateTileRules(std::array<int, 3>& upRules, std::array<int, 3>& downRules, std::array<int, 3>& leftRules, std::array<int, 3>& rightRules);
    void ProcessTiles();
};

