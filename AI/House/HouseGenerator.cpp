#include "HouseGenerator.h"
#include "../Pathfinding/AStar.h"
#include "../AIPlayroom.h"
#include "House.h"

void HouseGenerator::GenerateWareHouse(AStar& navmesh,AIPlayroom& room)
{

	//Warehouse
	House* Warehouse = new House();
	GenerateHorizontalWall("WarehouseBrick", navmesh, room, 0, 15, 32);
	GenerateHorizontalWall("WarehouseBrick", navmesh, room, 0, 15, 27);
	GenerateVerticalWall("WarehouseBrick", navmesh, room, 14, 25, 29);
	GenerateVerticalWall("WarehouseBrick", navmesh, room, 28, 31, 0);
	navmesh.markObstacle(15, 31);
	navmesh.markObstacle(15, 29);
	navmesh.markObstacle(15, 28);
	room.CreateEntity("WarehouseBrick", Vec2(15, 31), "Brick");
	room.CreateEntity("WarehouseBrick", Vec2(15, 29), "Brick");
	room.CreateEntity("WarehouseBrick", Vec2(15, 28), "Brick");
	MarkAreaAsHouse(navmesh, 1, 14, 28, 31);
	navmesh.navMesh[15][30].insideHouse = true;
}

void HouseGenerator::GenerateLHouse(AStar& navmesh, AIPlayroom& room)
{
	//25,30 - 32,30
	House* LHouse = new House();
	GenerateHorizontalWall("LHouseBrick", navmesh, room, 25, 32, 30);
	// 25,29
	// 25,27
	navmesh.markObstacle(25, 29);
	navmesh.markObstacle(25, 27);
	room.CreateEntity("LHouseBrick", Vec2(25, 29), "Brick");
	room.CreateEntity("LHouseBrick", Vec2(25, 27), "Brick");
	//25, 36 - 29, 26
	//29,35 - 29,14
	GenerateVerticalWall("LHouseBrick", navmesh, room, 14, 25, 29);
	//30,14
	//31,14
	navmesh.markObstacle(30, 14);
	navmesh.markObstacle(31, 14);
	room.CreateEntity("LHouseBrick", Vec2(30, 14), "Brick");
	room.CreateEntity("LHouseBrick", Vec2(31, 14), "Brick");
	//32,29 - 32,14
	GenerateVerticalWall("LHouseBrick", navmesh, room, 14, 29, 32);
	MarkAreaAsHouse(navmesh, 26, 31, 27, 29);
	navmesh.navMesh[25][28].insideHouse = true;
	MarkAreaAsHouse(navmesh, 30, 31, 15, 26);
	GenerateHorizontalWall("LHouseBrick", navmesh, room, 25, 29, 26);
}

void HouseGenerator::GenerateEightHouse(AStar& navmesh, AIPlayroom& room)
{
	House* EightHouse = new House();
	GenerateHorizontalWall("EightHouseBrick", navmesh, room, 8, 11, 22);
	GenerateHorizontalWall("EightHouseBrick", navmesh, room, 8, 13, 13);
	GenerateHorizontalWall("EightHouseBrick", navmesh, room, 10, 12, 18);
	GenerateVerticalWall("EightHouseBrick", navmesh, room, 14, 22, 8);
	GenerateVerticalWall("EightHouseBrick", navmesh, room, 14, 22, 13);

	MarkAreaAsHouse(navmesh, 9, 12, 19, 21);
	MarkAreaAsHouse(navmesh, 9, 12, 14, 17);

	navmesh.navMesh[9][18].insideHouse = true;
	navmesh.navMesh[12][22].insideHouse = true;
}

void HouseGenerator::GenerateMansion(AStar& navmesh, AIPlayroom& room)
{
	House* Mansion = new House();
	GenerateHorizontalWall("MansionBrick", navmesh, room, 13, 32, 9);
	GenerateHorizontalWall("MansionBrick", navmesh, room, 13, 32, 0);
	GenerateHorizontalWall("MansionBrick", navmesh, room, 19, 24, 5);
	GenerateHorizontalWall("MansionBrick", navmesh, room, 26, 28, 5);
	GenerateHorizontalWall("MansionBrick", navmesh, room, 30, 31, 5);
	GenerateVerticalWall("MansionBrick", navmesh, room, 6, 8, 13);
	GenerateVerticalWall("MansionBrick", navmesh, room, 1, 4, 13);
	GenerateVerticalWall("MansionBrick", navmesh, room, 1, 3, 19);
	GenerateVerticalWall("MansionBrick", navmesh, room, 6, 8, 19);
	GenerateVerticalWall("MansionBrick", navmesh, room, 6, 8, 26);
	GenerateVerticalWall("MansionBrick", navmesh, room, 2, 4, 28);
	GenerateVerticalWall("MansionBrick", navmesh, room, 1, 8, 32);


	MarkAreaAsHouse(navmesh, 14, 18, 1, 8);
	MarkAreaAsHouse(navmesh, 20, 25, 6, 8);
	MarkAreaAsHouse(navmesh, 20, 27, 1, 4);
	MarkAreaAsHouse(navmesh, 27, 31, 6, 8);
	MarkAreaAsHouse(navmesh, 29, 31, 1, 4);


	navmesh.navMesh[13][5].insideHouse = true;
	navmesh.navMesh[19][4].insideHouse = true;
	navmesh.navMesh[25][5].insideHouse = true;
	navmesh.navMesh[28][1].insideHouse = true;
	navmesh.navMesh[29][5].insideHouse = true;
}

void HouseGenerator::GenerateVerticalWall(std::string tag, AStar& navmesh, AIPlayroom& room,int y1, int y2, int x)
{
	for (int y = y1; y <= y2; y++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick");
	}
}

void HouseGenerator::GenerateHorizontalWall(std::string tag, AStar& navmesh, AIPlayroom& room, int x1, int x2, int y)
{
	for (int x = x1; x <= x2; x++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick");
	}
}

void HouseGenerator::MarkAreaAsHouse(AStar& navmesh, int x1, int x2, int y1, int y2)
{
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			navmesh.navMesh[x][y].insideHouse = true;
		}
	}
}
