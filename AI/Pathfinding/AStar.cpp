#include "AStar.h"
#include <unordered_set>
#include <iostream>
void AStar::initializeNavMesh()
{
	navMesh.resize(NAVMESH_HEIGHT, std::vector<Node>(NAVMESH_WIDTH, Node({ Vec2(0,0)}, true)));

	for (int y = 0; y < NAVMESH_HEIGHT; y++)
	{
		
		for (int x = 0; x < NAVMESH_WIDTH; x++)
		{
			navMesh[x][y] = Node(Vec2(x, y), true); //assume all nodes are walkable by default
		}
	}

	//Marking obstactales
	markObstacles();
}

void AStar::markObstacles()
{
	
	markObstacle(5,2);
	markObstacle(5, 3);
	markObstacle(5, 4);
	markObstacle(5, 5);
	markObstacle(5, 6);
	markObstacle(5, 7);
	markObstacle(5, 8);
	markObstacle(5, 9);

	markObstacle(5, 10);
	markObstacle(5, 11);
	markObstacle(5, 12);
	markObstacle(5, 13);
	markObstacle(5, 14);
	markObstacle(5, 15);
	markObstacle(5, 16);
	markObstacle(5, 17);
	markObstacle(5, 18);
	markObstacle(5,19);
}

void AStar::markObstacle(int x, int y)
{
	navMesh[x][y].walkable = false;
}

std::vector<Vec2> AStar::FindPath(Vec2 startPos, Vec2 endPos)
{
	Node* startNode = &navMesh[startPos.x][startPos.y];
	Node* endNode = &navMesh[endPos.x][endPos.y];

	std::vector<Node*>openList;
	std::unordered_set<Node*> closedList;
	
	openList.push_back(startNode);

	while (!openList.empty())
	{
		//Find the node with the lowest FCost
		auto currentNode = *std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b) {return a->fCost < b->fCost; });

		//Remove it from the open list and add it to the closed list
		openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
		closedList.insert(currentNode);

		//If we've reached the end node, reconstruct the path
		if (currentNode == endNode)
		{
			std::vector<Vec2> path;
			while (currentNode)
			{
				path.push_back(currentNode->position);
				currentNode = currentNode->parent;
			}
			std::reverse(path.begin(), path.end());
			for (Vec2 pathNode : path) {
				std::cout << pathNode.x << " , " << pathNode.y<<std::endl;
			}
			return path;
		}

		//Process neighbors
		std::vector<Vec2> directions = { {0,-1},{0,1},{-1,0},{1,0} };//Up,Down,Left,Right
		for (auto& dir : directions)
		{
			Vec2 neighbourPos = currentNode->position + dir;
			if (neighbourPos.x < 0 || neighbourPos.y < 0 || 
				neighbourPos.x >= NAVMESH_WIDTH || neighbourPos.y >= NAVMESH_HEIGHT)
			{
				continue;
			}

			Node* neighbour = &navMesh[neighbourPos.x][neighbourPos.y];

			if (!neighbour->walkable || closedList.count(neighbour))
			{
				continue;
			}

			int newGCost = currentNode->gCost + 1;
			if (newGCost < neighbour->gCost || std::find(openList.begin(), openList.end(), neighbour) == openList.end())
			{
				neighbour->gCost = newGCost;
				neighbour->CalculateCosts(endNode);
				neighbour->parent = currentNode;
				
				if (std::find(openList.begin(), openList.end(), neighbour) == openList.end())
				{
					openList.push_back(neighbour);
				}
			}
		}

	}
	std::cout << "NO PATH FOUND" << std::endl;
	//no path found
	return {};
	//std::vector<Vec2>();
}

void AStar::DrawPath(sf::RenderWindow& window)
{// Get the window height to flip the y-axis
	Vec2 windowSize = Vec2(window.getSize().x, window.getSize().y);
	for (int y = 0; y < NAVMESH_HEIGHT; ++y) {
		for (int x = 0; x < NAVMESH_WIDTH; ++x) {
			sf::RectangleShape cell(sf::Vector2f(64, 64));
			cell.setPosition(x * 64, windowSize.y - (y + 1) * 64);
			cell.setFillColor(navMesh[x][y].walkable ? sf::Color::Black : sf::Color::Red);
			window.draw(cell);
		}
	}
}
