#pragma once
#include <vector>
#include <SFML/System.hpp>
#include "AStar.h" 
class Vec2;
class BoustrophedonPathfinder
{
public:
    // Returns a list of positions covering the area
    static std::vector<Vec2> GeneratePath(const sf::IntRect& areaBounds, const std::vector<std::vector<AStar::Node>>& navMesh);

private:
    static bool IsCellWalkableAndUnscanned(const std::vector<std::vector<AStar::Node>>& navMesh, const Vec2& cell);
};

