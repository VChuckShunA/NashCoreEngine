#include "HouseGenerator.h"
#include "../Pathfinding/AStar.h"
#include "../AIPlayroom.h"
#include "House.h"

void HouseGenerator::GenerateWareHouse(AStar& navmesh,AIPlayroom& room)
{

	//Warehouse

	House* Warehouse = new House();
	for (int x = 0; x <= 15; x++)
	{
		navmesh.markObstacle(x, 32);
		room.CreateEntity("Brick", Vec2(x, 32), "Brick");
	}

	for (int x = 0; x <= 15; x++)
	{
		navmesh.markObstacle(x, 27);
		room.CreateEntity("Brick", Vec2(x, 27), "Brick");
	}

	for (int y = 28; y <= 31; y++)
	{
		navmesh.markObstacle(0, y);
		room.CreateEntity("Brick", Vec2(0, y), "Brick");
	}

	navmesh.markObstacle(15, 31);
	navmesh.markObstacle(15, 29);
	navmesh.markObstacle(15, 28);
	room.CreateEntity("Brick", Vec2(15, 31), "Brick");
	room.CreateEntity("Brick", Vec2(15, 29), "Brick");
	room.CreateEntity("Brick", Vec2(15, 28), "Brick");

	for (int x = 1; x <= 14; x++)
	{
		for (int y = 28; y <= 31; y++)
		{
			navmesh.navMesh[x][y].insideHouse = true;
		}
	}
	navmesh.navMesh[15][30].insideHouse = true;


}

void HouseGenerator::GenerateLHouse(AStar& navmesh, AIPlayroom& room)
{
	//25,30 - 32,30
	GenerateHorizontalWall(navmesh, room, 25, 32, 30);
	/*for (int x = 25; x <= 32; x++)
	{
		navmesh.markObstacle(x, 30);
		room.CreateEntity("Brick", Vec2(x, 30), "Brick");
	}*/
	// 25,29
	// 25,27
	// 
	navmesh.markObstacle(25, 29);
	navmesh.markObstacle(25, 27);
	room.CreateEntity("Brick", Vec2(25, 29), "Brick");
	room.CreateEntity("Brick", Vec2(25, 27), "Brick");
	//25, 36 - 29, 26
	GenerateHorizontalWall(navmesh, room, 25, 29, 26);
	//for (int x = 25; x <= 29; x++)
	//{
	//	navmesh.markObstacle(x, 26);
	//	room.CreateEntity("Brick", Vec2(x, 26), "Brick");
	//}
	//29,35 - 29,14
	GenerateVerticalWall(navmesh, room, 14, 25, 29);
	//for (int y = 14; y <= 25; y++)
	//{
	//	navmesh.markObstacle(29, y);
	//	room.CreateEntity("Brick", Vec2(29, y), "Brick");
	//}
	//30,14
	//31,14
	navmesh.markObstacle(30, 14);
	navmesh.markObstacle(31, 14);
	room.CreateEntity("Brick", Vec2(30, 14), "Brick");
	room.CreateEntity("Brick", Vec2(31, 14), "Brick");
	//32,29 - 32,14
	GenerateVerticalWall(navmesh, room, 14, 29, 32);
	/*for (int y = 14; y <= 29; y++)
	{
		navmesh.markObstacle(32, y);
		room.CreateEntity("Brick", Vec2(32, y), "Brick");
	}*/

	for (int x = 26; x <= 31; x++)
	{
		for (int y = 27; y <= 29; y++)
		{
			navmesh.navMesh[x][y].insideHouse = true;
		}
	}


		for (int y = 15; y <= 26; y++)
		{
			navmesh.navMesh[30][y].insideHouse = true;
			navmesh.navMesh[31][y].insideHouse = true;
		}

}

void HouseGenerator::GenerateEightHouse(AStar& navmesh, AIPlayroom& room)
{
	GenerateHorizontalWall(navmesh, room, 8, 11, 22);
	GenerateHorizontalWall(navmesh, room, 8, 13, 13);
	GenerateHorizontalWall(navmesh, room, 10, 12, 18);
	GenerateVerticalWall(navmesh, room, 14, 22, 8);
	GenerateVerticalWall(navmesh, room, 14, 22, 13);
}

void HouseGenerator::GenerateMansion(AStar& navmesh, AIPlayroom& room)
{
	GenerateHorizontalWall(navmesh, room, 13, 32, 9);
	GenerateHorizontalWall(navmesh, room, 13, 32, 0);
	GenerateHorizontalWall(navmesh, room, 19, 24, 5);
	GenerateHorizontalWall(navmesh, room, 26, 28, 5);
	GenerateHorizontalWall(navmesh, room, 30, 31, 5);
	GenerateVerticalWall(navmesh, room, 6, 8, 13);
	GenerateVerticalWall(navmesh, room, 1, 4, 13);
	GenerateVerticalWall(navmesh, room, 1, 3, 19);
	GenerateVerticalWall(navmesh, room, 6, 8, 19);
	GenerateVerticalWall(navmesh, room, 2, 4, 26);
	GenerateVerticalWall(navmesh, room, 6, 8, 28);
}

void HouseGenerator::GenerateVerticalWall(AStar& navmesh, AIPlayroom& room,int y1, int y2, int x)
{
	for (int y = y1; y <= y2; y++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick");
	}
}

void HouseGenerator::GenerateHorizontalWall(AStar& navmesh, AIPlayroom& room, int x1, int x2, int y)
{
	for (int x = x1; x <= x2; x++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick");
	}
}
