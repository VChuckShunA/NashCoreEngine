#include "HouseGenerator.h"
#include "../Pathfinding/AStar.h"
#include "../AIPlayroom.h"
#include "House.h"
std::vector<std::shared_ptr<House>> generatedHouses;
void HouseGenerator::GenerateWareHouse(AStar& navmesh,AIPlayroom& room)
{
	////Patching the doors for now
	//navmesh.markObstacle(12, 22);
	//navmesh.markObstacle(25, 28);
	//navmesh.markObstacle(13, 5);
	//room.CreateEntity("Brick", Vec2(12, 22), "Brick");
	//room.CreateEntity("Brick", Vec2(25, 28), "Brick");
	//room.CreateEntity("Brick", Vec2(13, 5), "Brick");
	//Warehouse
	auto Warehouse = std::make_shared<House>();
	Warehouse->houseID = 1;
	Warehouse->houseLabel = "Warehouse";
	GenerateHorizontalWall(1, navmesh, room, 0, 15, 32);
	GenerateHorizontalWall(1, navmesh, room, 0, 15, 27);
	GenerateVerticalWall(1, navmesh, room, 28, 31, 0);
	navmesh.markObstacle(15, 31);
	navmesh.markObstacle(15, 29);
	navmesh.markObstacle(15, 28);
	room.CreateEntity("WarehouseBrick", Vec2(15, 31), "Brick", 1);
	room.CreateEntity("WarehouseBrick", Vec2(15, 29), "Brick", 1);
	room.CreateEntity("WarehouseBrick", Vec2(15, 28), "Brick", 1);
	MarkAreaAsHouse(navmesh, 1, 14, 28, 31);
	navmesh.navMesh[15][30].insideHouse = true;
	navmesh.navMesh[15][30].scannable = true;
	Warehouse->AddMainDoor(Vec2(15, 30));
	Warehouse->AddBounds(31,1,14,4);
	generatedHouses.push_back(Warehouse);
}

void HouseGenerator::GenerateLHouse(AStar& navmesh, AIPlayroom& room)
{
	//25,30 - 32,30
	auto LHouse = std::make_shared<House>();
	LHouse->houseID = 2;
	LHouse->houseLabel = "LHouse";
	GenerateHorizontalWall(2, navmesh, room, 25, 32, 30);
	// 25,29
	// 25,27
	navmesh.markObstacle(25, 29);
	navmesh.markObstacle(25, 27);
	room.CreateEntity("LHouseBrick", Vec2(25, 29), "Brick",2);
	room.CreateEntity("LHouseBrick", Vec2(25, 27), "Brick",2);
	//25, 36 - 29, 26
	//29,35 - 29,14
	GenerateVerticalWall(2, navmesh, room, 14, 25, 29);
	//30,14
	//31,14
	navmesh.markObstacle(30, 14);
	navmesh.markObstacle(31, 14);
	room.CreateEntity("Brick", Vec2(30, 14), "Brick", 2);
	room.CreateEntity("Brick", Vec2(31, 14), "Brick", 2);
	//32,29 - 32,14
	GenerateVerticalWall(2, navmesh, room, 14, 29, 32);
	MarkAreaAsHouse(navmesh, 26, 31, 27, 29);
	navmesh.navMesh[25][28].insideHouse = true;
	navmesh.navMesh[25][28].scannable = true;
	MarkAreaAsHouse(navmesh, 30, 31, 15, 26);
	GenerateHorizontalWall(2, navmesh, room, 25, 29, 26);
	LHouse->AddMainDoor(Vec2(25, 28));
	LHouse->AddBounds(29, 26, 6, 15);
	generatedHouses.push_back(LHouse);
}

void HouseGenerator::GenerateEightHouse(AStar& navmesh, AIPlayroom& room)
{
	auto EightHouse = std::make_shared<House>();
	EightHouse->houseID = 3;
	EightHouse->houseLabel = "EightHouse";
	GenerateHorizontalWall(3, navmesh, room, 8, 11, 22);
	GenerateHorizontalWall(3, navmesh, room, 8, 13, 13);
	GenerateHorizontalWall(3, navmesh, room, 10, 12, 18);
	GenerateVerticalWall(3, navmesh, room, 14, 22, 8);
	GenerateVerticalWall(3, navmesh, room, 14, 22, 13);

	MarkAreaAsHouse(navmesh, 9, 12, 19, 21);
	MarkAreaAsHouse(navmesh, 9, 12, 14, 17);

	navmesh.navMesh[9][18].insideHouse = true;
	navmesh.navMesh[12][22].insideHouse = true;


	navmesh.navMesh[9][18].scannable = true;
	navmesh.navMesh[12][22].scannable = true;

	EightHouse->AddMainDoor(Vec2(12, 22));
	EightHouse->AddBounds(21, 9, 4, 8);
	generatedHouses.push_back(EightHouse);

}

void HouseGenerator::GenerateMansion(AStar& navmesh, AIPlayroom& room)
{
	auto Mansion = std::make_shared<House>();
	Mansion->houseID = 4;
	Mansion->houseLabel = "Mansion";
	GenerateHorizontalWall(4, navmesh, room, 13, 32, 9);
	GenerateHorizontalWall(4, navmesh, room, 13, 32, 0);
	GenerateHorizontalWall(4, navmesh, room, 19, 24, 5);
	GenerateHorizontalWall(4, navmesh, room, 26, 28, 5);
	GenerateHorizontalWall(4, navmesh, room, 30, 31, 5);
	GenerateVerticalWall(4, navmesh, room, 6, 8, 13);
	GenerateVerticalWall(4, navmesh, room, 1, 4, 13);
	GenerateVerticalWall(4, navmesh, room, 1, 3, 19);
	GenerateVerticalWall(4, navmesh, room, 6, 8, 19);
	GenerateVerticalWall(4, navmesh, room, 6, 8, 26);
	GenerateVerticalWall(4, navmesh, room, 2, 4, 28);
	GenerateVerticalWall(4, navmesh, room, 1, 8, 32);


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

	navmesh.navMesh[13][5].scannable = true;
	navmesh.navMesh[19][4].scannable = true;
	navmesh.navMesh[25][5].scannable = true;
	navmesh.navMesh[28][1].scannable = true;
	navmesh.navMesh[29][5].scannable = true;

	Mansion->AddMainDoor(Vec2(13, 5));
	Mansion->AddBounds(8, 14, 18, 8);
	//Room 1
	auto door1 = std::make_shared<House::Door>();
	door1->doorPosition = Vec2(19,4);
	std::vector<std::shared_ptr<House::Door>> room1Doors;
	room1Doors.push_back(door1);
	Mansion->AddRoom(8,14,5,8, room1Doors,Mansion.get(), 1);

	//Room 2
	auto door2 = std::make_shared<House::Door>();
	door2->doorPosition = Vec2(25, 5);
	auto door3 = std::make_shared<House::Door>();
	door3->doorPosition = Vec2(28, 1);
	std::vector<std::shared_ptr<House::Door>> room2Doors;
	room2Doors.push_back(door2);
	room2Doors.push_back(door3);
	Mansion->AddRoom(4, 20, 8, 4, room2Doors, Mansion.get(), 2);

	//Room 3
	std::vector<std::shared_ptr<House::Door>> room3Doors;
	Mansion->AddRoom(8, 20, 6, 3, room3Doors, Mansion.get(), 3);

	//Room 4
	auto door4 = std::make_shared<House::Door>();
	door4->doorPosition = Vec2(29, 5);
	std::vector<std::shared_ptr<House::Door>> room4Doors;
	Mansion->AddRoom(4, 29, 3, 4, room4Doors, Mansion.get(), 4);

	//Room 5
	std::vector<std::shared_ptr<House::Door>> room5Doors;
	Mansion->AddRoom(8, 27, 5, 3, room5Doors, Mansion.get(), 5);
	generatedHouses.push_back(Mansion);
}

void HouseGenerator::GenerateVerticalWall(int id, AStar& navmesh, AIPlayroom& room,int y1, int y2, int x)
{
	for (int y = y1; y <= y2; y++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick",id);
	}
}

void HouseGenerator::GenerateHorizontalWall(int id, AStar& navmesh, AIPlayroom& room, int x1, int x2, int y)
{
	for (int x = x1; x <= x2; x++)
	{
		navmesh.markObstacle(x, y);
		room.CreateEntity("Brick", Vec2(x, y), "Brick",id);
	}
}

void HouseGenerator::MarkAreaAsHouse(AStar& navmesh, int x1, int x2, int y1, int y2)
{
	for (int x = x1; x <= x2; x++)
	{
		for (int y = y1; y <= y2; y++)
		{
			navmesh.navMesh[x][y].insideHouse = true;
			navmesh.navMesh[x][y].scannable = true;
		}
	}
}

House* HouseGenerator::FindHouseByID(int id)
{
	for (auto& house : generatedHouses)
	{
		if (house->houseID == id)
			return house.get();
	}
	return nullptr;
}
