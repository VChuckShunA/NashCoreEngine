#include "BoustrophedonPathfinder.h"
#include "../../vec2.h"
#include <iostream>

std::vector<Vec2> BoustrophedonPathfinder::GeneratePath(const sf::IntRect& areaBounds, const std::vector<std::vector<AStar::Node>>& navMesh)
{
    std::vector<Vec2> path;
    bool leftToRight = true;
    for (int y = areaBounds.top; y < areaBounds.top + areaBounds.height; ++y) {
        if (leftToRight) {
            for (int x = areaBounds.left; x < areaBounds.left + areaBounds.width; ++x) {
                Vec2 cell(x, y);
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                    path.push_back(cell);
                }
            }
        }
        else {
            for (int x = areaBounds.left + areaBounds.width - 1; x >= areaBounds.left; --x) {
                Vec2 cell(x, y);
                if (IsCellWalkableAndUnscanned(navMesh, cell)) {
                    path.push_back(cell);
                }
            }
        }
        leftToRight = !leftToRight;
    }

    return path;
}

bool BoustrophedonPathfinder::IsCellWalkableAndUnscanned(const std::vector<std::vector<AStar::Node>>& navMesh, const Vec2& cell)
{
    std::cout << "staradasd" << std::endl;
    return navMesh[cell.x][cell.y].walkable && navMesh[cell.x][cell.y].insideHouse;
}
