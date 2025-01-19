#include "Action.h"
#include "SceneMenu.h"
#include "ScenePlay.h"
#include "Vec2.h"
#include "Physics.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
    init(levelPath);


    
}

void Scene_Play::init(const std::string& levelPath) {
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid

    // Register all other gameplay Actions
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");

    m_gridText.setCharacterSize(12);
    // m_gridText.setFont(m_game->assets().getFont("Mario"));
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    loadLevel(levelPath);

}

void Scene_Play::loadLevel(const std::string& fileName) {
    // reset the entity manager every time we load a level
    m_entityManager = EntityManager();

    // read in the level file and add the appropriate entities
    // use the PlayerConfig struct m_playerConfig to store player properties
    // this struct is defined at the top of Scene_Play.h
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Scene_Play::loadLevel could not load " << fileName << " file.\n";
        exit(-1);
    }
    //Implement WFC here
    std::cout << "Start WFC" << std::endl;

    //ImplementWFC(grid);
    std::srand(std::time(nullptr));

    std::cout << &grid << std::endl;
    // Generate a random number from the TileType enum
    int randomTile = std::rand() % 14; // 14 because the enum has 14 values (0-13)

    // Generate a random position on the grid
    int randomRow = std::rand() % 19;  // 20 rows
    int randomCol = std::rand() % 12;  // 12 columns

    // Output results
    std::cout << "Random Tile: " << randomTile << std::endl;
    std::cout << "Random Position: (" << randomRow << ", " << randomCol << ")" << std::endl;
   
    SpiralTraverse(grid);
    std::cout << "End WFC" << std::endl;

    std::string entityType;
    while (file >> entityType) {
        if (entityType == "Tile") {
            std::string animationName;
            float gridX, gridY;
            file >> animationName >> gridX >> gridY;

            auto tile = m_entityManager.addEntity("tile");
            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            tile->addComponent<CTransform>(
                gridToMidPixel(gridX, gridY, tile),
                Vec2(0, 0),
                Vec2(1, 1),
                0
            );
            tile->addComponent<CBoundingBox>(m_gridSize);

        }
        else if (entityType == "Dec") {
            std::string animationName;
            float gridX, gridY;
            file >> animationName >> gridX >> gridY;

            auto dec = m_entityManager.addEntity("dec");
            dec->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            dec->addComponent<CTransform>(
                gridToMidPixel(gridX, gridY, dec),
                Vec2(0, 0),
                Vec2(1, 1),
                0
            );

        }
        else if (entityType == "Player") {
            file >> m_playerConfig.X >> m_playerConfig.Y
                >> m_playerConfig.CX >> m_playerConfig.CY
                >> m_playerConfig.SPEED
                >> m_playerConfig.JUMP
                >> m_playerConfig.MAX_SPEED
                >> m_playerConfig.GRAVITY
                >> m_playerConfig.WEAPON;
            spawnPlayer();

        }
        else {
            std::cerr << "Unknown entity type " << entityType << "\n";
            // exit(-1);
        }

        
    }

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       Any changes you make to transform1 will not be changed inside the entity
    //       auto transform1 = entity->get<CTransform>()
    //
    //       This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    //       Now any changes you make to transform2 will be changed inside the entity
    //       auto& transform2 = entity->get<CTransform>()
}

void Scene_Play::ImplementWFC(TileState(&grid)[20][12]){
    // Seed for randomness
    std::srand(std::time(nullptr));

    std::cout << &grid << std::endl;
    // Generate a random number from the TileType enum
    int randomTile = std::rand() % 14; // 14 because the enum has 14 values (0-13)

    // Generate a random position on the grid
    int randomRow = std::rand() % 19;  // 20 rows
    int randomCol = std::rand() % 12;  // 12 columns

    // Output results
    std::cout << "Random Tile: " << randomTile << std::endl;
    std::cout << "Random Position: (" << randomRow << ", " << randomCol << ")" << std::endl;

    
    RenderTile(&randomTile, &randomRow, &randomCol);
    

    UpdateRuleSet(&grid[randomRow][randomCol], randomRow, randomCol);
    Collapse(randomRow, randomCol);
}

void Scene_Play::Collapse(int currentX,int currentY)
{
     int randomIndex;
     int selectedTile;
     int possibleTileSize;
     int newX, newY;
    for (int x = 1; x < 20; x++) 
    {
        for (int y = 1; y < 13; y++) 
        {
            if (!grid[currentX][currentY + y].collapsed)
            {
                
                if ((currentY + y) <= 12)
                {
                    /*for (int tile : grid[currentX][currentY + y].possibleTiles)
                    {
                        std::cout << currentX << " , " << currentY + y << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX][currentY + y].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX][currentY+y], currentX, currentY + y);
                    }
                    if (!grid[currentX][currentY + y].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX][currentY + y].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX][currentY + y].possibleTiles[randomIndex];
                        newX = currentX;
                        newY = currentY + y;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX][currentY + y].collapsed = true;
                    }
                }
                UpdateRuleSet(&grid[currentX][currentY + y], currentX, currentY + y);
            }
            if (!grid[currentX][currentY-y].collapsed)
            {
                
                if ((currentY - y) >= 0)
                {
                    /*for (int tile : grid[currentX][currentY - y].possibleTiles)
                    {
                        std::cout << currentX << " , " << currentY - y << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX][currentY-y].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX][currentY-y], currentX, currentY-y);
                    }
                    if (!grid[currentX][currentY - y].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX][currentY - y].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX][currentY - y].possibleTiles[randomIndex];
                        newX = currentX;
                        newY = currentY - y;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX][currentY - y].collapsed = true;
                    }
                }
                UpdateRuleSet(&grid[currentX][currentY - y], currentX, currentY - y);
             
            }
            if (!grid[currentX+ x][currentY].collapsed)
            {
                
               // std::cout<< "x: " << currentX +x << " , y: " << currentY<<std::endl;
                if ((currentX + x)<=19)
                {
                    /*for (int tile : grid[currentX + x][currentY].possibleTiles)
                    {
                        std::cout << currentX + x << " , " << currentY << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX + x][currentY].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX + x][currentY], currentX + x, currentY);
                    }
                    if (!grid[currentX + x][currentY].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX + x][currentY].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX + x][currentY].possibleTiles[randomIndex];
                        newX = currentX + x;
                        newY = currentY;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX + x][currentY].collapsed = true;
                    }
                }
                UpdateRuleSet(&grid[currentX+x][currentY ], currentX+x, currentY);

            }
            if (!grid[currentX - x][currentY].collapsed)
            {
                if ((currentX - x) >= 0)
                {
                    /*for (int tile : grid[currentX - x][currentY].possibleTiles)
                    {
                        std::cout << currentX - x << " , " << currentY << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX - x][currentY].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX - x][currentY], currentX - x, currentY);
                    }
                    if (!grid[currentX - x][currentY].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX - x][currentY].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX - x][currentY].possibleTiles[randomIndex];
                        newX = currentX - x;
                        newY = currentY;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX - x][currentY].collapsed = true;
                    }
                }

                UpdateRuleSet(&grid[currentX - x][currentY], currentX - x, currentY);
            }

            if (!grid[currentX - x][currentY-y].collapsed)
            {
                if ((currentX - x) >= 0&&(currentY - y)>=0)
                {
                    /*for (int tile : grid[currentX - x][currentY-y].possibleTiles)
                    {
                        std::cout << currentX - x << " , " << currentY-y<< " : " << tile << std::endl;
                    }*/
                    if (grid[currentX - x][currentY - y].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX - x][currentY - y], currentX - x, currentY - y);
                    }
                    if (!grid[currentX - x][currentY-y].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX - x][currentY-y].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX - x][currentY-y].possibleTiles[randomIndex];
                        newX = currentX - x;
                        newY = currentY-y;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX - x][currentY-y].collapsed = true;
                    }
                }

                UpdateRuleSet(&grid[currentX - x][currentY-y], currentX - x, currentY-y);
            }

            if (!grid[currentX + x][currentY + y].collapsed)
            {
                if ((currentX + x) <= 12 && (currentY + y) <= 19)
                {
                    /*for (int tile : grid[currentX + x][currentY + y].possibleTiles)
                    {
                        std::cout << currentX + x << " , " << currentY + y << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX + x][currentY + y].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX + x][currentY + y], currentX + x, currentY + y);
                    }
                    if (!grid[currentX + x][currentY + y].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX + x][currentY + y].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;


                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX + x][currentY + y].possibleTiles[randomIndex];
                        newX = currentX + x;
                        newY = currentY + y;
                        RenderTile(&selectedTile, &newX, &newY);
                        grid[currentX + x][currentY + y].collapsed = true;
                    }
                }

                UpdateRuleSet(&grid[currentX + x][currentY + y], currentX + x, currentY + y);
            }

            if (!grid[currentX + x][currentY - y].collapsed)
            {
                if ((currentX + x) <= 12 && (currentY - y) >= 0)
                {
                    /*for (int tile : grid[currentX + x][currentY - y].possibleTiles)
                    {
                        std::cout << currentX + x << " , " << currentY - y << " : " << tile << std::endl;
                    }*/
                    if (grid[currentX + x][currentY - y].possibleTiles.empty())
                    {
                        UpdateRuleSet(&grid[currentX + x][currentY - y], currentX + x, currentY - y);
                    }
                    if (!grid[currentX + x][currentY - y].possibleTiles.empty()) {
                        possibleTileSize = grid[currentX + x][currentY - y].possibleTiles.size();
                        std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;

                        std::cout << "TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! " << std::endl;
                        std::cout << "SHOULD RENDER AT : " << currentX + x << " , " << currentY - y << std::endl;

                        randomIndex = std::rand() % possibleTileSize;
                        selectedTile = grid[currentX + x][currentY - y].possibleTiles[randomIndex];
                        newX = currentX + x;
                        newY = currentY - y;
                        RenderTile(&selectedTile, &newX, &newY);
                        std::cout << "RENDERED AT : " << newX << " , "<<newY << std::endl;
                        grid[currentX + x][currentY - y].collapsed = true;
                    }
                }
                UpdateRuleSet(&grid[currentX+x][currentY - y], currentX+x, currentY - y);
            }

            //if (!grid[currentX - x][currentY + y].collapsed)
            //{
            //    if ((currentX - x) <= 12 && (currentY + y) >= 0)
            //    {
            //        
            //        if (grid[currentX - x][currentY + y].possibleTiles.empty())
            //        {
            //            UpdateRuleSet(&grid[currentX - x][currentY + y], currentX - x, currentY + y);
            //        }/*for (int tile : grid[currentX - x][currentY + y].possibleTiles)
            //        {
            //            std::cout << currentX - x << " , " << currentY + y << " : " << tile << std::endl;
            //        }*/
            //        if (!grid[currentX - x][currentY + y].possibleTiles.empty()) {
            //            possibleTileSize = grid[currentX - x][currentY + y].possibleTiles.size();
            //            std::cout << "Possible Tiles Size : " << possibleTileSize << std::endl;

            //            std::cout << "TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! TRUE! " << std::endl;
            //            std::cout << "SHOULD RENDER AT : " << currentX - x << " , " << currentY + y << std::endl;

            //            randomIndex = std::rand() % possibleTileSize;
            //            selectedTile = grid[currentX - x][currentY + y].possibleTiles[randomIndex];
            //            newX = currentX - x;
            //            newY = currentY + y;
            //            RenderTile(&selectedTile, &newX, &newY);
            //            std::cout << "RENDERED AT : " << newX << " , " << newY << std::endl;
            //            grid[currentX - x][currentY + y].collapsed = true;
            //        }
            //    }
            //    UpdateRuleSet(&grid[currentX-x][currentY + y], currentX-x, currentY + y);
            //}
        }
    }
}

void Scene_Play::SpiralTraverse(TileState(&grid)[20][12])
{
    const int N = 20; // Number of rows
    const int M = 12; // Number of columns

    // Seed for random number generation
    std::srand(std::time(0));

    // Pick a random starting point
    int startRow = std::rand() % N;
    int startCol = std::rand() % M;

    std::cout << "Starting at (" << startRow << ", " << startCol << ")\n";

    // Direction vectors for movement (dx, dy)
    int directions[4][2] = {
        {0, 1},   // Right
        {1, 0},   // Down
        {0, -1},  // Left
        {-1, 0}   // Up
    };

    // Spiral traversal parameters
    int layer = 0;       // Current layer of the spiral
    int dirIndex = 0;    // Direction index
    int steps = 1;       // Steps in the current direction
    int x = startCol;    // Current column
    int y = startRow;    // Current row
    int randomTile = 4;
    // Traverse the grid
    while (layer <= std::max(N, M)) {
        for (int step = 0; step < steps; ++step) {
            // Check if the current cell is within bounds
            if (x >= 0 && x < M && y >= 0 && y < N) {
                // Process the grid cell
                TileState& tile = grid[y][x];
                std::cout << "Visiting (" << y << ", " << x << "): ";
                RenderTile(&randomTile,&y,&x);
                if (!tile.collapsed && !tile.possibleTiles.empty()) {
                    // Collapse the tile by assigning the first possible tile
                    tile.currentTile = tile.possibleTiles[0];
                    tile.collapsed = true;
                    std::cout << "Collapsed, Assigned Tile " << tile.currentTile;
                }
                else {
                    std::cout << "Already collapsed or no possible tiles.";
                }
                std::cout << std::endl;
            }

            // Move to the next cell
            x += directions[dirIndex][0];
            y += directions[dirIndex][1];
        }

        // Rotate direction (0 -> 1 -> 2 -> 3 -> 0)
        dirIndex = (dirIndex + 1) % 4;

        // Update the number of steps and layer when changing direction
        if (dirIndex == 0 || dirIndex == 2) {
            ++steps;
        }

        // Break when we have traversed beyond the largest layer
        if (x < -layer || x > layer + M || y < -layer || y > layer + N) {
            break;
        }

        // Increase the layer size after completing all four directions
        if (dirIndex == 3) {
            ++layer;
        }
    }
}


void Scene_Play::UpdateCommonElements(int x, int y, std::vector<TileType>* rulesToCompare)
{
    // Sort both vectors to use set_intersection
    std::sort(grid[x][y].possibleTiles.begin(), grid[x][y].possibleTiles.end());
    std::sort(rulesToCompare->begin(), rulesToCompare->end());

    // Vector to store the common elements
    std::vector<int> commonElements;

    // Find the intersection of possibleTiles and upRules
    std::set_intersection(
        grid[x][y].possibleTiles.begin(), grid[x][y].possibleTiles.end(),
        rulesToCompare->begin(), rulesToCompare->end(),
        std::back_inserter(commonElements)
    );

    // Replace possibleTiles with the common elements
    grid[x][y].possibleTiles = commonElements;

    // Print the updated possibleTiles
    std::cout << "Updated possibleTiles for (x=" << x << ", y=" << y << "): ";
    for (int tile : grid[x][y].possibleTiles) {
        std::cout << tile << " ";
    }
    std::cout << std::endl;
}

void Scene_Play::UpdateRuleSet(TileState* cell,int x ,int y)
{ 
    int left = x - 1;
    int right = x + 1;
    int up = y - 1;
    int down = y+1;
    std::vector<TileType> leftRules, rightRules, upRules, downRules;
    //extract rules
    if (adjacencyRules.find(static_cast<TileType>(cell->currentTile)) != adjacencyRules.end()) {
        const auto& rules = adjacencyRules[static_cast<TileType>(cell->currentTile)];

        // Iterate over the directions and print them
        for (const auto& [direction, tileList] : rules) {
            //std::cout << direction << ": ";
            //for (const auto& tile : tileList) {
            //    std::cout << tile << " "; // Prints the TileType enum as int
            //}
            //std::cout << std::endl;
            //
            if (direction == "up")
            {
                upRules = tileList;
            }
            else if (direction == "down")
            {
                downRules = tileList;
            }
            else if (direction == "left")
            {
                leftRules = tileList;
            }
            else if (direction == "right")
            {
                rightRules = tileList;
            }
        }
    }
    else {
        std::cout << "No rules found for this tile "<<x << " "<< y << std::endl;
    }

    //Update possible tiles
    if (x > 0 && x < 19 && y > 0 && y < 12) // Ensure bounds are valid
    {
        //Up tiles
        if (!grid[x][up].collapsed)
        {
            if (grid[x][up].possibleTiles.empty())
            {
                grid[x][up].possibleTiles.insert(
                    grid[x][up].possibleTiles.end(),
                    upRules.begin(),
                    upRules.end()
                );
               /* std::cout << "No existing rules were found, and new Up rules were added." << std::endl;
                std::cout << "Up Rules: ";
               */ /*for (int rule : grid[x][up].possibleTiles) {
                    std::cout << rule << " , ";
                }
                std::cout<< std::endl;*/
            }
            else if (!grid[x][up].possibleTiles.empty())
            {
               // std::cout << "Updating Common Up Rules" << std::endl;
                UpdateCommonElements(x, up, &upRules);
            }
            
        }
        //Right tiles
        if (!grid[right][y].collapsed)
        {
            if (grid[right][y].possibleTiles.empty())
            {
                grid[right][y].possibleTiles.insert(
                    grid[right][y].possibleTiles.end(),
                    rightRules.begin(),
                    rightRules.end()
                );

               /* std::cout << "No existing rules were found, and new Right rules were added." << std::endl;
                std::cout << "Right Rules: ";*/
                /*for (int rule : grid[right][y].possibleTiles) {
                    std::cout << rule << " , ";
                }
                std::cout << std::endl;*/
            }
            else if (!grid[right][y].possibleTiles.empty())
            {
                /*std::cout << "Updating Common Right Rules" << std::endl;*/
                UpdateCommonElements(right, y, &rightRules);
            }
        }

        //Down tiles
        if (!grid[x][down].collapsed)
        {
            if (grid[x][down].possibleTiles.empty())
            {
                grid[x][down].possibleTiles.insert(
                    grid[x][down].possibleTiles.end(),
                    downRules.begin(),
                    downRules.end()
                );
                /*std::cout << "No existing rules were found, and new Down rules were added." << std::endl;
                std::cout << "Down Rules: ";
                for (int rule : grid[x][down].possibleTiles) {
                    std::cout << rule << " , ";
                }
                std::cout << std::endl;*/
            }
            else if (!grid[x][down].possibleTiles.empty())
            {
               // std::cout << "Updating Common Down Rules" << std::endl;
                UpdateCommonElements(x, down, &downRules);
            }
        }

        //Left tiles
        if (!grid[left][y].collapsed)
        {
            if (grid[left][y].possibleTiles.empty())
            {
                grid[left][y].possibleTiles.insert(
                    grid[left][y].possibleTiles.end(),
                    leftRules.begin(),
                    leftRules.end()
                );/*
                std::cout << "No existing rules were found, and new Left rules were added." << std::endl;
                std::cout << "Left Rules: ";
                for (int rule : grid[left][y].possibleTiles) {
                    std::cout << rule << " , ";
                }
                std::cout << std::endl;*/
            }
            else if (!grid[left][y].possibleTiles.empty())
            {
                //std::cout << "Updating Common left Rules" << std::endl;
                UpdateCommonElements(left, y, &leftRules);
            }
        }
    }
    else
    {
        std::cerr << "Error: Right or y index out of bounds!" << x << " , "<< y << std::endl;
    }
}

void Scene_Play::RenderTile(int* tileID,int* randomRow, int* randomCol)
{
    auto dec = m_entityManager.addEntity("dec");
    switch (*tileID)
    {
    case BRIDGE:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Bridge"), true);
        break;
    case COMPONENT:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Component"), true);
        break;
    case CONNECTION:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Connection"), true);
        break;
    case CORNER:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Corner"), true);
        break;
    case DSKEW:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("DSkew"), true);
        break;
    case SKEW:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Skew"), true);
        break;
    case SUBSTRATE:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Substrate"), true);
        break;
    case T:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("T"), true);
        break;
    case TRACK:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Track"), true);
        break;
    case TRANSITION:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Transition"), true);
        break;
    case TURN:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Turn"), true);
        break;
    case VIAD:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("ViaD"), true);
        break;
    case VIAS:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("ViaS"), true);
        break;
    case WIRE:
        dec->addComponent<CAnimation>(m_game->assets().getAnimation("Wire"), true);
        break;
    }

    grid[*randomRow][*randomCol].collapsed = true;
    grid[*randomRow][*randomCol].currentTile = *tileID;
   // dec->getComponent<CAnimation>().animation.setSize(Vec2(64, 64));
    dec->addComponent<CTransform>(
        gridToMidPixel(*randomRow, *randomCol, dec),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    std::cout << "Rendered tile " << *tileID << "at " << *randomRow << " , " << *randomCol << std::endl;
}



Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, const std::shared_ptr<Entity>& entity) {
    // This function takes in a grid (x,y) position and an Entity
    // Return a vec2 indicating where the CENTER position of the Entity should be
    // You must use the Entity's Animation size to position it correctly
    // The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    // The bottom-left corner of the Animation should align with the bottom left of the grid cell

    auto entitySize = entity->getComponent<CAnimation>().animation.getSize();
    // vec2 offset = m_gridSize - entitySize;
    return {
            gridX * m_gridSize.x + entitySize.x / 2.0f,
            height() - gridY * m_gridSize.y - entitySize.y / 2.0f
    };
    //    return {
    //            gridX * m_gridSize.x + m_gridSize.x / 2.0f - offset.x,
    //            height() - gridY * m_gridSize.y - m_gridSize.y / 2.0f + offset.y
    //    };
}

void Scene_Play::spawnPlayer() {
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Stand"), true);
    m_player->addComponent<CTransform>(
        gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player),
        Vec2(m_playerConfig.SPEED, 0),
        Vec2(1, 1),
        0
    );
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));

    // be sure to add the remaining components to the player
    m_player->addComponent<CInput>();
    m_player->addComponent<CState>("stand");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
}

void Scene_Play::spawnBullet(const std::shared_ptr<Entity>& entity) {
    // this should spawn a bullet at the given entity, going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.WEAPON), true);
    // vec2(30,-3) is a tweak so that bullet starts at the end of gun; it is determined experimentally
    float dir = 1.0f;
    if (entity->getComponent<CTransform>().scale.x < 0) dir = -1.0;
    bullet->addComponent<CTransform>(
        entity->getComponent<CTransform>().pos/* + vec2(30,-3) */,
        Vec2(dir * 2 * m_playerConfig.SPEED, 0),
        // vec2(5 * entity->getComponent<CTransform>().scale.x, 0),
        entity->getComponent<CTransform>().scale,
        0
    );
    bullet->addComponent<CLifespan>(90, m_currentFrame);
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
}

void Scene_Play::update() {
    m_entityManager.update();

    // implement pause functionality
    if (!m_paused) {
        sMovement();
        sLifespan();
        sCollision();
        m_currentFrame++;
    }
    sAnimation();
    sRender();
}

void Scene_Play::sMovement() {
    // Implement player movement/jumping based on its CInput component
    // Implement gravity's effect on the player
    // Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
    // reset player speed to zero
    m_player->getComponent<CTransform>().velocity.x = 0;

    if (m_player->getComponent<CInput>().left) {
        m_player->getComponent<CTransform>().velocity.x = -m_playerConfig.SPEED;
        if (m_player->getComponent<CTransform>().scale.x > 0) {
            m_player->getComponent<CTransform>().scale.x = -1;
        }
    }
    else if (m_player->getComponent<CInput>().right) {
        m_player->getComponent<CTransform>().velocity.x = m_playerConfig.SPEED;
        if (m_player->getComponent<CTransform>().scale.x < 0) {
            m_player->getComponent<CTransform>().scale.x = 1;
        }
    }

    if (m_player->getComponent<CInput>().up) {
        if (m_player->getComponent<CInput>().canJump) {
            m_player->getComponent<CInput>().canJump = false;
            m_player->getComponent<CTransform>().velocity.y = -m_playerConfig.JUMP;
        }
    }
    //    else if (m_player->getComponent<CTransform>().velocity.y <= 0) {
    //        m_player->getComponent<CTransform>().velocity.y = 0;
    //    }

    if (m_player->getComponent<CInput>().shoot) {
        if (m_player->getComponent<CInput>().canShoot) {
            spawnBullet(m_player);
            m_player->getComponent<CInput>().canShoot = false;
        }
    }
    else {
        m_player->getComponent<CInput>().canShoot = true;
    }

    // update all entities positions
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->hasComponent<CGravity>()) {
            Vec2& v = entity->getComponent<CTransform>().velocity;
            v.y += entity->getComponent<CGravity>().gravity;
            if (v.y > m_playerConfig.MAX_SPEED) {
                v.y = m_playerConfig.MAX_SPEED;
            }
        }
        entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
        entity->getComponent<CTransform>().pos += entity->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::sLifespan() {
    // Check lifespan of entities that have them, and destroy them if they go over
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->hasComponent<CLifespan>()) {
            if (entity->getComponent<CAnimation>().animation.getName() == "CoinSpin") {
                if (entity->getComponent<CAnimation>().animation.hasEnded()) {
                    entity->destroy();
                }
            }
            else {
                auto& eLife = entity->getComponent<CLifespan>();
                if (m_currentFrame - eLife.frameCreated >= eLife.lifespan) {
                    entity->destroy();
                }
            }
        }
    }

    // control bullet quantity, be spawned every 10 frames
    for (const auto& entity : m_entityManager.getEntities("bullet")) {
        auto& bulletLife = entity->getComponent<CLifespan>();
        if (m_currentFrame - bulletLife.frameCreated == 20) {
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void Scene_Play::sCollision() {
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // Implement Physics::GetOverlap() function, use it inside this function

    // Implement bullet/tile collisions
    // Destroy the tile if it has a Brick animation
    for (const auto& bullet : m_entityManager.getEntities("bullet")) {
        for (const auto& tile : m_entityManager.getEntities("tile")) {
            // check bullet and tile side collide
            Vec2 overlap = Physics::GetOverlap(bullet, tile);
            Vec2 pOverlap = Physics::GetPreviousOverlap(bullet, tile);
            if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                if (0 <= overlap.x && pOverlap.x <= 0) {
                    if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                        spawnBrickDebris(tile);
                    }
                    bullet->destroy();
                }
            }
        }
    }

    // Implement player/tile collisions and resolutions
    // Update the CState component of the player to store whether
    // it is currently on the ground or in the air. This will be
    // used by the Animation system
    // reset gravity
    m_player->getComponent<CGravity>().gravity = m_playerConfig.GRAVITY;
    for (const auto& tile : m_entityManager.getEntities("tile")) {
        Vec2 overlap = Physics::GetOverlap(m_player, tile);
        Vec2 pOverlap = Physics::GetPreviousOverlap(m_player, tile);
        // check if player is in air
        // check tiles being below player
        float dy = tile->getComponent<CTransform>().pos.y - m_player->getComponent<CTransform>().pos.y;
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy > 0) {
            if (0 <= overlap.y && pOverlap.y <= 0) {
                // stand on tile
                m_player->getComponent<CInput>().canJump = true;
                m_player->getComponent<CGravity>().gravity = 0;
                m_player->getComponent<CTransform>().velocity.y = 0;
                // collision resolution
                m_player->getComponent<CTransform>().pos.y -= overlap.y;
            }
        }
        // check if player hits the tile from the bottom
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy < 0) {
            if (0 <= overlap.y && pOverlap.y <= 0) {
                m_player->getComponent<CTransform>().pos.y += overlap.y;
                m_player->getComponent<CTransform>().velocity.y = 0;
                if (tile->getComponent<CAnimation>().animation.getName() == "Question") {
                    tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("QuestionHit");
                    spawnCoinSpin(tile);
                }
                if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                    spawnBrickDebris(tile);
                }
            }
        }
        // check player and tile side collide
        float dx = tile->getComponent<CTransform>().pos.x - m_player->getComponent<CTransform>().pos.x;
        if (0 < overlap.y && -m_gridSize.x < overlap.x) {
            if (0 <= overlap.x && pOverlap.x <= 0) {
                if (dx > 0) {
                    // tile is right of player
                    m_player->getComponent<CTransform>().pos.x -= overlap.x;
                }
                else {
                    // tile is left of player
                    m_player->getComponent<CTransform>().pos.x += overlap.x;
                }
            }
        }
    }

    // Check to see if the player has fallen down a hole (y > height())
    if (m_player->getComponent<CTransform>().pos.y > height()) {
        m_player->getComponent<CTransform>().pos = gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
    }

    // Don't let the player walk off the left side of the map
    if (m_player->getComponent<CTransform>().pos.x < m_player->getComponent<CBoundingBox>().size.x / 2.0f) {
        m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CBoundingBox>().size.x / 2.0f;
    }
}

void Scene_Play::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }

        else if (action.name() == "JUMP") {
            if (m_player->getComponent<CInput>().canJump) { m_player->getComponent<CInput>().up = true; }
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = true;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "SHOOT") {
            if (m_player->getComponent<CInput>().canShoot) {
                m_player->getComponent<CInput>().shoot = true;
            }
        }
    }
    else if (action.type() == "END") {
        if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "SHOOT") {
            m_player->getComponent<CInput>().shoot = false;
        }
    }
}

void Scene_Play::sAnimation() {
    // Complete the Animation class code first
    // Set the animation of the player based on its CState component
    // check player state
    if (m_player->getComponent<CTransform>().velocity.y != 0) {
        m_player->getComponent<CInput>().canJump = false;
        if (m_player->getComponent<CInput>().shoot) {
            changePlayerStateTo("airshoot");
        }
        else {
            changePlayerStateTo("air");
        }
    }
    else {
        if (m_player->getComponent<CTransform>().velocity.x != 0) {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo("runshoot");
            }
            else {
                changePlayerStateTo("run");
            }
        }
        else {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo("standshoot");
            }
            else {
                changePlayerStateTo("stand");
            }
        }
    }

    // change player animation
    if (m_player->getComponent<CState>().changeAnimation) {
        std::string animationName;
        if (m_player->getComponent<CState>().state == "stand") {
            animationName = "Stand";
        }
        else if (m_player->getComponent<CState>().state == "air") {
            animationName = "Jump";
        }
        else if (m_player->getComponent<CState>().state == "run") {
            animationName = "Run";
        }
        else if (m_player->getComponent<CState>().state == "standshoot") {
            animationName = "StandShoot";
        }
        else if (m_player->getComponent<CState>().state == "airshoot") {
            animationName = "AirShoot";
        }
        else if (m_player->getComponent<CState>().state == "runshoot") {
            animationName = "RunShoot";
        }
        // std::cout << "Ivan: getAnimation " << animationName << "\n";
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
    }

    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->getComponent<CAnimation>().animation.hasEnded() && !entity->getComponent<CAnimation>().repeat) {
            entity->destroy();
        }
        if (entity->hasComponent<CAnimation>()) {
            entity->getComponent<CAnimation>().animation.update();
        }
    }
    // call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
}

void Scene_Play::onEnd() {
    // when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::sRender() {
    // color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto& pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = std::max(float(m_game->window().getSize().x) / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, float(m_game->window().getSize().y) - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto& e : m_entityManager.getEntities()) {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (const auto& e : m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_drawGrid) {
        float leftX = float(m_game->window().getView().getCenter().x) - width() / 2.0f;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - float((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += float(m_gridSize.y)) {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

void Scene_Play::changePlayerStateTo(const std::string& state) {
    auto& prev = m_player->getComponent<CState>().previousState;
    if (prev != state) {
        prev = m_player->getComponent<CState>().state;
        m_player->getComponent<CState>().state = state;
        m_player->getComponent<CState>().changeAnimation = true;
    }
    else {
        m_player->getComponent<CState>().changeAnimation = false;
    }
}

void Scene_Play::spawnBrickDebris(const std::shared_ptr<Entity>& tile) {
    tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Explosion");
    // tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("BrickDebris");
    tile->addComponent<CLifespan>(10, m_currentFrame);
}

void Scene_Play::spawnCoinSpin(const std::shared_ptr<Entity>& tile) {
    auto coin = m_entityManager.addEntity("coinspin");
    coin->addComponent<CAnimation>(
        m_game->assets().getAnimation("CoinSpin"),
        false
    );
    coin->addComponent<CTransform>(
        Vec2(
            tile->getComponent<CTransform>().pos.x,
            tile->getComponent<CTransform>().pos.y - m_gridSize.y
        ),
        Vec2(0, 0),
        tile->getComponent<CTransform>().scale,
        0
    );
    coin->addComponent<CLifespan>(30, m_currentFrame);
}
