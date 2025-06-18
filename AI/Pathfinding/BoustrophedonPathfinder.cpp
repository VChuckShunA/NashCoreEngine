#include "BoustrophedonPathfinder.h"
#include "../../vec2.h"
#include "AStar.h" 
#include <iostream>

std::vector<Vec2> BoustrophedonPathfinder::GeneratePath(const sf::FloatRect& areaBounds, const std::vector<std::vector<AStar::Node>>& navMesh)
{
    std::vector<Vec2> path;
    bool leftToRight = true;
    for (int y = areaBounds.top; y > areaBounds.top - areaBounds.height; --y) {
        if (leftToRight)
        {
            for (int x = areaBounds.left; x < areaBounds.left + areaBounds.width; ++x) {
              //  std::cout << "Cell : " << x << " , " << y << std::endl;
                Vec2 cell(x, y);
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                    path.push_back(cell);
                }
            }
        }
        else
        { 
            for (int x = areaBounds.left + areaBounds.width - 1; x >= areaBounds.left; --x) {
                Vec2 cell(x, y);
               // std::cout << "Cell : " << cell.x << " , " << cell.y << std::endl;
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                      path.push_back(cell);
                }
            }
        }
        leftToRight = !leftToRight;
    }
   

    return path;

    /*std::vector<Vec2> path;
    bool leftToRight = true;
    for (int y = areaBounds.top; y < areaBounds.top - areaBounds.height; ++y) {
        if (leftToRight) {
            for (int x = areaBounds.left; x < areaBounds.left + areaBounds.width; ++x) {
                Vec2 cell(x, y);
                std::cout << "Cell : " << cell.x << " , " << cell.y << std::endl;
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                    path.push_back(cell);
                }
            }
        }
        else {
            for (int x = areaBounds.left + areaBounds.width - 1; x >= areaBounds.left; --x) {
                Vec2 cell(x, y);
                std::cout << "Cell : " << cell.x << " , " << cell.y << std::endl;
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                    path.push_back(cell);
                }
            }
        }
        leftToRight = !leftToRight;
    }

    return path;*/
}

bool BoustrophedonPathfinder::IsCellWalkableAndUnscanned(const std::vector<std::vector<AStar::Node>>& navMesh, const Vec2& cell)
{
    int x = cell.x;
    int y = cell.y;

    // Always check bounds before accessing
    if (y < 0 || y >= 33) return false;
    if (x < 0 || x >= 33) return false;

    const AStar::Node& node = navMesh[x][y];
    return node.walkable && node.insideHouse;
}
