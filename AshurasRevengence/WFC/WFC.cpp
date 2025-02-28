#include "WFC.h"
#include "Tile.h"
#include <unordered_map>
#include "../../vec2.h"
#include "../../EntityManager.h"
#include "../../Scene.h"

WFC::WFC(AshuraLevel& AshuralevelInstance):
    levelInstance(AshuralevelInstance)
{
    tileInstance = Tile();
    ProcessTiles();
    ImplementWFC(tileInstance.grid);
}

void WFC::RenderTile(Tile::TileType tileID, int* randomRow, int* randomCol)
{
    auto dec = levelInstance.m_entityManager.addEntity("dec");
    

    tileInstance.grid[*randomRow][*randomCol].collapsed = true;
    tileInstance.grid[*randomRow][*randomCol].currentTile = tileID;
    Tile::TileType tile = tileID;


    tileInstance.grid[*randomRow][*randomCol].sockets.up = tileInstance.adjacencyRules[tile].adjacencyRules["up"];
    tileInstance.grid[*randomRow][*randomCol].sockets.down = tileInstance.adjacencyRules[tile].adjacencyRules["down"];
    tileInstance.grid[*randomRow][*randomCol].sockets.left = tileInstance.adjacencyRules[tile].adjacencyRules["left"];
    tileInstance.grid[*randomRow][*randomCol].sockets.right = tileInstance.adjacencyRules[tile].adjacencyRules["right"];

    dec->addComponent<CAnimation>(levelInstance.m_game->assets().getAnimation(tileInstance.adjacencyRules.at(tileID).textureName), true);


    dec->addComponent<CTransform>(
        levelInstance.gridToMidPixel(*randomRow, *randomCol, dec),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );

    if (tileInstance.adjacencyRules.at(tileID).rotationCount > 0)
    {
        dec->getComponent<CTransform>().angle = dec->getComponent<CTransform>().angle + (90 * tileInstance.adjacencyRules.at(tileID).rotationCount);

    }
    // std::cout << "Rotated " << rotationCount << " times !" << std::endl;
    UpdateNeighbourRules(*randomRow, *randomCol);
    UpdatePossibleTiles(*randomRow - 1, *randomCol);
    UpdatePossibleTiles(*randomRow + 1, *randomCol);
    UpdatePossibleTiles(*randomRow, *randomCol - 1);
    UpdatePossibleTiles(*randomRow, *randomCol + 1);
}

void WFC::Collapse()
{
    std::pair<int, int> tileToCollapse = FindLowestEntropy();
    if (tileInstance.grid[tileToCollapse.first][tileToCollapse.second].possibleTiles.empty()) {
        if ((tileToCollapse.first >= 0) && (tileToCollapse.second >= 0))
        {
            ResetGrid();

            return;
        }
        std::cerr << "No possible tiles for the FOLLOWING " << tileToCollapse.first << ", " << tileToCollapse.second << "\n";
        return;
    }
    if (tileToCollapse.first < 0 || tileToCollapse.first > 20 || tileToCollapse.second < 0 || tileToCollapse.second > 12)
    {
        return;
    }

    UpdatePossibleTiles(tileToCollapse.first, tileToCollapse.second);
    int randomTileID = std::rand() % tileInstance.grid[tileToCollapse.first][tileToCollapse.second].possibleTiles.size();
    Tile::TileType randomTile = tileInstance.grid[tileToCollapse.first][tileToCollapse.second].possibleTiles[randomTileID];

    RenderTile(randomTile, &tileToCollapse.first, &tileToCollapse.second);
    Collapse();
}

std::pair<int, int> WFC::FindLowestEntropy()
{
    int minEntropy = INT_MAX;
    std::vector<std::pair<int, int>> candidates;

    for (int y = 0; y < 12; ++y) {  // Iterate over rows (height)
        for (int x = 0; x < 20; ++x) {  // Iterate over columns (width)
            Tile::TileState& cell = tileInstance.grid[x][y];

            if (!cell.collapsed) {  // Only consider non-collapsed cells
                int entropy = cell.possibleTiles.size();  // Count valid tile choices

                if (entropy < minEntropy) {
                    minEntropy = entropy;
                    candidates.clear();
                    candidates.emplace_back(x, y);
                }
                else if (entropy == minEntropy) {
                    candidates.emplace_back(x, y);
                }
            }
        }
    }

    if (candidates.empty()) {
        return { -1, -1 };  // No available cell to collapse
    }

    // Randomly pick a tile from the candidates with the lowest entropy
    return candidates[rand() % candidates.size()];
}

void WFC::UpdateNeighbourRules(int currentX, int currentY)
{
    if (tileInstance.grid[currentX][currentY].collapsed)
    {

        if (currentX > 0)
            tileInstance.grid[currentX - 1][currentY].sockets.right = tileInstance.grid[currentX][currentY].sockets.left;
        if (currentX < 19)
            tileInstance.grid[currentX + 1][currentY].sockets.left = tileInstance.grid[currentX][currentY].sockets.right;
        if (currentY > 0)
            tileInstance.grid[currentX][currentY - 1].sockets.up = tileInstance.grid[currentX][currentY].sockets.down;
        if (currentY < 11)
            tileInstance.grid[currentX][currentY + 1].sockets.down = tileInstance.grid[currentX][currentY].sockets.up;

    }
}

void WFC::UpdatePossibleTiles(int currentX, int currentY)
{
    if (currentX < 0 || currentX >= 20 || currentY < 0 || currentY >= 12) {
        return; // Prevent out-of-bounds access
    }

    // std::cout << "UPDATING POSSIBLE TILES FOR " << currentX << " , " << currentY << std::endl;

     // Lambda function to determine whether a tile should be removed
    auto tileFilter = [&](Tile::TileType tile) {
        if (tileInstance.adjacencyRules.find(tile) == tileInstance.adjacencyRules.end()) {
            return true; // Remove if no adjacency rules exist
        }

        bool matches = true; // Assume tile is valid until proven otherwise

        // Check LEFT side
        if (tileInstance.grid[currentX][currentY].sockets.left != std::array<int, 3>{90, 90, 90}) {
            if (tileInstance.adjacencyRules[tile].adjacencyRules["left"] != tileInstance.grid[currentX][currentY].sockets.left) {
                matches = false; // Invalidate if the left side doesn't match
            }
        }

        // Check RIGHT side
        if (tileInstance.grid[currentX][currentY].sockets.right != std::array<int, 3>{90, 90, 90}) {
            if (tileInstance.adjacencyRules[tile].adjacencyRules["right"] != tileInstance.grid[currentX][currentY].sockets.right) {
                matches = false;
            }
        }

        // Check UP side
        if (tileInstance.grid[currentX][currentY].sockets.up != std::array<int, 3>{90, 90, 90}) {
            if (tileInstance.adjacencyRules[tile].adjacencyRules["up"] != tileInstance.grid[currentX][currentY].sockets.up) {
                matches = false;
            }
        }

        // Check DOWN side
        if (tileInstance.grid[currentX][currentY].sockets.down != std::array<int, 3>{90, 90, 90}) {
            if (tileInstance.adjacencyRules[tile].adjacencyRules["down"] != tileInstance.grid[currentX][currentY].sockets.down) {
                matches = false;
            }
        }

        return !matches; // Remove the tile if any direction doesn't match
    };

    // Use remove_if with the lambda function
    tileInstance.grid[currentX][currentY].possibleTiles.erase(
        std::remove_if(
            tileInstance.grid[currentX][currentY].possibleTiles.begin(),
            tileInstance.grid[currentX][currentY].possibleTiles.end(),
            tileFilter
        ),
        tileInstance.grid[currentX][currentY].possibleTiles.end()
    );
}

void WFC::ImplementWFC(Tile::TileState(&grid)[20][12])
{
    const int N = 20; // Number of rows
    const int M = 12; // Number of columns

    // Seed for random number generation
    std::srand(std::time(nullptr));

    // Pick a random starting point
    int startRow = std::rand() % N;
    int startCol = std::rand() % M;
    Tile::TileType randomTile = static_cast<Tile::TileType>(std::rand() % 14); // 14 because the enum has 14 values (0-13)
    // std::cout << "Starting at (" << startRow << ", " << startCol << ")\n";
    int startx = startRow, starty = startCol;
    RenderTile(randomTile, &startx, &starty);


    int x = startx;    // Current row
    int y = starty;    // Current column

    grid[x][y].sockets.up = tileInstance.adjacencyRules[randomTile].adjacencyRules["up"];
    grid[x][y].sockets.down = tileInstance.adjacencyRules[randomTile].adjacencyRules["down"];
    grid[x][y].sockets.left = tileInstance.adjacencyRules[randomTile].adjacencyRules["left"];
    grid[x][y].sockets.right = tileInstance.adjacencyRules[randomTile].adjacencyRules["right"];

    UpdateNeighbourRules(x, y);

    UpdatePossibleTiles(startRow - 1, startCol);
    UpdatePossibleTiles(startRow + 1, startCol);
    UpdatePossibleTiles(startRow, startCol - 1);
    UpdatePossibleTiles(startRow, startCol + 1);
    Collapse();
}

void WFC::ResetGrid()
{
    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 12; y++)
        {
            tileInstance.grid[x][y].sockets.up = { 90, 90, 90 };
            tileInstance.grid[x][y].sockets.down = { 90, 90, 90 };
            tileInstance.grid[x][y].sockets.left = { 90, 90, 90 };
            tileInstance.grid[x][y].sockets.right = { 90, 90, 90 };
            tileInstance.grid[x][y].possibleTiles = {
                Tile::BRIDGE, Tile::COMPONENT, Tile::CONNECTION, Tile::CORNER, Tile::DSKEW, Tile::SKEW, Tile::SUBSTRATE, Tile::T, Tile::TRACK, Tile::TRANSITION, Tile::TURN, Tile::VIAD, Tile::VIAS, Tile::WIRE,
                    Tile::BRIDGE1, Tile::BRIDGE2, Tile::BRIDGE3,
                    Tile::CONNECTION1, Tile::CONNECTION2, Tile::CONNECTION3,
                    Tile::CORNER1, Tile::CORNER2, Tile::CORNER3,
                    Tile::DSKEW1, Tile::DSKEW2, Tile::DSKEW3,
                    Tile::SKEW1, Tile::SKEW2, Tile::SKEW3,
                    Tile::T1, Tile::T2, Tile::T3,
                    Tile::TRACK1, Tile::TRACK2, Tile::TRACK3,
                    Tile::TRANSITION1, Tile::TRANSITION2, Tile::TRANSITION3,
                    Tile::TURN1, Tile::TURN2, Tile::TURN3,
                    Tile::VIAD1, Tile::VIAD2, Tile::VIAD3,
                    Tile::VIAS1, Tile::VIAS2, Tile::VIAS3,
                    Tile::WIRE1, Tile::WIRE2, Tile::WIRE3,
            };
            tileInstance.grid[x][y].collapsed = false;
            tileInstance.grid[x][y].currentTile = NULL;
        }
    }
    for (const auto& entity : levelInstance.m_entityManager.getEntities("dec")) {

        entity->destroy();


    }
    ImplementWFC(tileInstance.grid);
}

void WFC::RotateTileRules(std::array<int, 3>& upRules, std::array<int, 3>& downRules, std::array<int, 3>& leftRules, std::array<int, 3>& rightRules)
{
    std::array<int, 3> rotUpRules = upRules; //1 1 1
    std::array<int, 3> rotdownRules = downRules; //9 1 1
    std::array<int, 3> rotLeftRules = leftRules; //1 1 9
    std::array<int, 3> rotRightRules = rightRules; //1 1 1
    std::array<int, 3> tempRules = { NULL,NULL,NULL };

    tempRules = rotUpRules; //1 1 1
    reverse(rotLeftRules.begin(), rotLeftRules.end()); // 9 1 1
    rotUpRules = rotLeftRules; // 9 1 1
    rotLeftRules = rotdownRules; //9 1 1
    reverse(rotRightRules.begin(), rotRightRules.end()); //  1 1 1
    rotdownRules = rotRightRules; // 1 1 1
    rotRightRules = tempRules; // 1 1 1

    upRules = rotUpRules; // 9 1 1
    downRules = rotdownRules; // 1 1 1
    leftRules = rotLeftRules; // 9 1 1
    rightRules = rotRightRules; // 1 1 1

}

void WFC::ProcessTiles()
{
    Tile::TileInfo newTileInfo;
    int newTile = 14;
    std::unordered_map<Tile::TileType, Tile::TileInfo> newAdjacencyRules;
    std::array<int, 3> newUpRules, newDownRules, newRightRules, newLeftRules;
    for (const auto& [tile, info] : tileInstance.adjacencyRules) {
        if (tile == Tile::COMPONENT || tile == Tile::SUBSTRATE) continue; // Skip these tile types

        newUpRules = tileInstance.adjacencyRules[tile].adjacencyRules["up"];
        newDownRules = tileInstance.adjacencyRules[tile].adjacencyRules["down"];
        newLeftRules = tileInstance.adjacencyRules[tile].adjacencyRules["left"];
        newRightRules = tileInstance.adjacencyRules[tile].adjacencyRules["right"];
        newTileInfo.textureName = info.textureName;


        for (int i = 1; i <= 3; i++) {
            RotateTileRules(newUpRules, newDownRules, newLeftRules, newRightRules);
            newTileInfo.rotationCount = i;
            newTileInfo.adjacencyRules["up"] = newUpRules;
            newTileInfo.adjacencyRules["down"] = newDownRules;
            newTileInfo.adjacencyRules["left"] = newLeftRules;
            newTileInfo.adjacencyRules["right"] = newRightRules;
            newAdjacencyRules.insert({ static_cast<Tile::TileType>(newTile),newTileInfo });
            newTile++;
        }


    }
    tileInstance.adjacencyRules.insert(newAdjacencyRules.begin(), newAdjacencyRules.end());
    for (const auto& [tile, info] : tileInstance.adjacencyRules) {

        std::cout << "TileType: " << static_cast<int>(tile)
            << " (" << info.textureName << "), Rotations: " << info.rotationCount << std::endl;

        for (const auto& [direction, ruleSet] : info.adjacencyRules) {
            std::cout << "  Direction: " << direction << " -> Rules: [ ";

            for (int rule : ruleSet) {
                std::cout << rule << " ";
            }

            std::cout << "]" << std::endl;
        }
    }
}
