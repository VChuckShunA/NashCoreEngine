#pragma once
#include <vector>
#include "../../vec2.h"
#include "../AIPlayroom.h"
class House
{
	friend class HouseGenerator;
private:
	struct Door
	{
		//bool scanned = false;
		Vec2 entryPoint1;
		Vec2 entryPoint2;
		bool scanned;
		Vec2 doorPosition;
		std::shared_ptr<class Room> connectedRoom;
		Vec2 GetEntryPoint(Vec2 position);
		void ResetDoor();
		void AddRoom(std::shared_ptr<Room> room);
	};
	struct Room
	{
		int roomID; 
		House* house;
		sf::IntRect bounds;
		std::vector<std::shared_ptr<Door>> doors;
		bool searched = false;
		std::vector<Vec2> roomSpace;
		Vec2 GetClosestRoom(Vec2 position);
		void ResetAndPopulateRoom();
		void AddDoor(Door* door);
		House* GetHouse() { return house; }
	};


	bool Searched = false;
	std::vector<std::shared_ptr<Door>> mainDoors;
public:
	int houseID;
	sf::IntRect bounds;
	std::vector<Room> rooms;
	std::string houseLabel; //L House, 8House,Warehouse, Mansion
	Vec2 GetClosestMainDoor(Vec2& agentPosition);
	Vec2 GetClosestEntryPoint();
	void ResetAndPopulateHouse();
	void AddMainDoor(Vec2 mainDoorCords);
	void AddRoom(int top, int left, int width, int height, std::vector<std::shared_ptr<Door>> doors, House* house, int roomID);
	void AddBounds(int top, int left, int width, int height);
	
	bool IsBoundsSet();
};

