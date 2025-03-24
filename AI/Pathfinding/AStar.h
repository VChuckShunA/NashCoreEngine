#pragma once
#include "../../vec2.h"
#include <cstdlib>
#include <vector>
class AStar
{
public:
	struct Node {
		Vec2 position;
		int gCost, hCost, fCost;
		bool walkable;
		Node* parent;

		Node(Vec2 pos,bool isWalkable) : position(pos),gCost(0),hCost(0),walkable(isWalkable),parent(nullptr){}
		void CalculateCosts(Node* endNode)
		{
			hCost = std::abs(position.x - endNode->position.x) + std::abs(position.y - endNode->position.y);
			fCost = gCost + hCost;
		}
	};

	const int NAVMESH_WIDTH = 20, NAVMESH_HEIGHT = 12;
	std::vector<std::vector<Node>> navMesh;

	void initializeNavMesh();
	void markObstacles();
	void markObstacle(int x, int y);

	std::vector<Vec2>FindPath(Vec2 startPos, Vec2 endPos);

};

