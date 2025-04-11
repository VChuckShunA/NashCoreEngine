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
	markObstacle(5, 20);
	markObstacle(5, 21);
	markObstacle(5, 22);
	markObstacle(5, 23);
	markObstacle(5, 24);
	markObstacle(5, 25);
	markObstacle(5, 26);
	markObstacle(5, 27);
	markObstacle(5, 28);
	markObstacle(5, 29);
	markObstacle(5, 30);
	markObstacle(5, 31);
	markObstacle(5, 32);
	markObstacle(5, 33);
	markObstacle(5, 34);
	markObstacle(5, 35);
	markObstacle(5, 36);
	markObstacle(5, 37);
	markObstacle(5, 38);
	markObstacle(5, 39);
	markObstacle(5, 40);
	markObstacle(12, 18);
	markObstacle(12, 17);
	markObstacle(12, 16);
	markObstacle(12, 15);
	markObstacle(12, 14);
	markObstacle(12, 13);
	markObstacle(12, 12);
	markObstacle(12, 11);
	markObstacle(12, 10);
	markObstacle(12, 9);
	markObstacle(12, 8);
	markObstacle(12, 7);
	markObstacle(12, 6);
	markObstacle(12, 5);
	markObstacle(6, 11);
	markObstacle(7, 11);
	markObstacle(8, 11);
	markObstacle(9, 11);
	markObstacle(10, 11);
	markObstacle(11, 11);
}

void AStar::markObstacle(int x, int y)
{
	navMesh[x][y].walkable = false;
}

std::vector<Vec2> AStar::FindPath(Vec2 startPos, Vec2 endPos, std::vector<Vec2>& outPath)
{   // Reset all nodes in the navMesh
	for (int x = 0; x < NAVMESH_WIDTH; ++x) {
		for (int y = 0; y < NAVMESH_HEIGHT; ++y) {
			Node& node = navMesh[x][y];
			node.parent = nullptr;
			node.gCost = std::numeric_limits<int>::max();
			node.fCost = std::numeric_limits<int>::max();
		}
	}
	outPath.clear();
	Node* startNode = &navMesh[startPos.x][startPos.y];
	Node* endNode = &navMesh[endPos.x][endPos.y];

	std::vector<Node*>openList;
	std::unordered_set<Node*> closedList;

	openList.push_back(startNode);

	while (!openList.empty())
	{
		std::cout << "A Star 103" << std::endl;
		//Find the node with the lowest FCost
		auto currentNode = *std::min_element(openList.begin(), openList.end(), [](Node* a, Node* b) {return a->fCost < b->fCost; });

		//Remove it from the open list and add it to the closed list
		openList.erase(std::remove(openList.begin(), openList.end(), currentNode), openList.end());
		closedList.insert(currentNode);

		std::cout << "A Star 110" << std::endl;
		//If we've reached the end node, reconstruct the path
		if (currentNode == endNode)
		{

			std::cout << "A Star 116" << std::endl;
			while (currentNode)
			{
				std::cout << "A Star 119" << currentNode << std::endl;
				outPath.push_back(currentNode->position);
				currentNode = currentNode->parent;
			}
			std::reverse(outPath.begin(), outPath.end());
			for (Vec2 pathNode : outPath) {
				std::cout << pathNode.x << " , " << pathNode.y<<std::endl;
			}
			currentNode = NULL;
			endNode = NULL;
			delete currentNode;
			delete endNode;
			std::cout << "A Star 127" << std::endl;
			return outPath;
		}

		//Process neighbors
		std::vector<Vec2> directions = { {0,-1},{0,1},{-1,0},{1,0},{1,1},{-1,-1},{1,-1},{-1,1} };//Up,Down,Left,Right
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
			cell.setFillColor(navMesh[x][y].walkable ? sf::Color::Transparent : sf::Color::Red);
			window.draw(cell);
		}
	}
}
