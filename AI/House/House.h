#pragma once
#include <vector>
#include "../../vec2.h"
#include "../AIPlayroom.h"
class House
{
private:
	struct Door
	{
		bool scanned = false;
		/*Vec2 entryPoint1;
		Vec2 entryPoint2;*/
		Vec2 doorPosition;
		Vec2 GetEntryPoint(Vec2 position);
		void ResetDoor();
	};
	struct Room
	{
		bool scanned = false;
		std::vector<Vec2> roomSpace;
		std::vector<Vec2> roomDoors;
		Vec2 GetClosestRoom(Vec2 position);
		void ResetAndPopulateRoom();
	};

	bool Searched = false;
	std::vector<std::shared_ptr<Room>> rooms;
	std::vector<std::shared_ptr<Door>> mainDoors;
public:
	int houseID;
	std::string houseLabel; //L House, 8House,Warehouse, Mansion
	Vec2 GetClosestMainDoor(Vec2& agentPosition);
	Vec2 GetClosestEntryPoint();
	void ResetAndPopulateHouse();
	void AddMainDoor(Vec2 mainDoorCords);
	void AddRoom(Vec2& topleft, Vec2& topRight, Vec2& Bottomleft, Vec2& BottomRight, std::vector<std::shared_ptr<Door>> doors);


};

