#pragma once
#include <vector>
#include "../../vec2.h"
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
using namespace std;
//#include "../AIPlayroom.h"

class House
{
	struct Room; //forward declare
	friend class HouseGenerator;
	friend class BaseAIAgent;
public:
	struct Door
	{
		//bool scanned = false;
		Vec2 entryPoint1;
		Vec2 entryPoint2;
		bool scanned;
		Vec2 doorPosition;
		std::shared_ptr<class Room> connectedRoom;
		Vec2 GetEntryPoint(Vec2 position);
		Vec2 GetExitPoint(Vec2 position);
		void ResetDoor();
		void AddRoom(std::shared_ptr<Room> room);
	};
	struct Room
	{

		friend class House;
		int roomID; 
		House* house;
		sf::FloatRect bounds;
		std::vector<std::shared_ptr<Door>> doors;
		bool searched = false;
		std::vector<Vec2> roomSpace;
		House::Door* GetClosestRoomDoor(Vec2 position);
		void ResetAndPopulateRoom();
		void AddDoor(Door* door);
		House* GetHouse() { return house; }
		std::shared_ptr<House::Door> AddRoomDoor(Vec2 mainDoorCords, bool horizontal = true);
	};


	std::vector<std::shared_ptr<Door>> mainDoors;
public:
	int houseID;
	bool Searched = false;
	sf::FloatRect bounds;
	std::vector<std::shared_ptr<Room>> rooms;
	std::string houseLabel; //L House, 8House,Warehouse, Mansion
	House::Door* GetClosestMainDoor(Vec2& agentPosition);
	Vec2 GetClosestEntryPoint();
	void ResetHouse();
	void AddMainDoor(Vec2 mainDoorCords,bool horizontal=true);
	std::shared_ptr<House::Room> AddRoom(int top, int left, int width, int height, House* house, int roomID);
	void AddBounds(int top, int left, int width, int height);
	std::shared_ptr<House::Room> FindCurrentRoom(Vec2 agentPosition);
	bool IsBoundsSet();
};

