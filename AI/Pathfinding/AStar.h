#pragma once
#include "../../vec2.h"
#include <cstdlib>
#include <vector>
#include <SFML/Graphics.hpp>
#include <array>
class AIPlayroom;
class AStar
{
public:
	struct Node {
		Vec2 position;
		int gCost, hCost, fCost;
		bool walkable;
		bool insideHouse;
		bool scannable;
		bool foribidden = false;
		Node* parent;

		Node(Vec2 pos,bool isWalkable) : position(pos),gCost(0),hCost(0),walkable(isWalkable),parent(nullptr){
			insideHouse = false;
			scannable = false;
		}
		void CalculateCosts(Node* endNode)
		{
			hCost = std::abs(position.x - endNode->position.x) + std::abs(position.y - endNode->position.y);
			fCost = gCost + hCost;
		}
	};



	const int NAVMESH_WIDTH = 33, NAVMESH_HEIGHT = 33;
	std::vector<std::vector<Node>> navMesh;

	void initializeNavMesh();
	void markObstacles();
	void markObstacle(int x, int y);

	std::vector<Vec2>FindPath(Vec2 startPos, Vec2 endPos);
	void DrawPath(sf::RenderWindow& window);
};

