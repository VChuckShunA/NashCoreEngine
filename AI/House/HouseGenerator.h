#pragma once
class AStar;
class AIPlayroom;
class House;
class HouseGenerator
{
public:
	static void GenerateWareHouse(AStar& navmesh,AIPlayroom& room);
	static void GenerateLHouse(AStar& navmesh, AIPlayroom& room);
	static void GenerateEightHouse(AStar& navmesh, AIPlayroom& room);
	static void GenerateMansion(AStar& navmesh, AIPlayroom& room);

	static void GenerateVerticalWall(std::string tag, AStar& navmesh, AIPlayroom& room,int y1,int y2, int x);

	static void GenerateHorizontalWall(std::string tag,AStar& navmesh, AIPlayroom& room, int x1, int x2, int y);
	static void MarkAreaAsHouse(AStar& navmesh, int x1, int x2, int y1,int y2);
};

