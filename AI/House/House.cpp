#include "House.h"

void House::Door::ResetDoor()
{
	scanned = false;
}

void House::AddRoom(Vec2& topleft, Vec2& topRight, Vec2& Bottomleft, Vec2& BottomRight, std::vector<std::shared_ptr<Door>> doors)
{
	//Room* newRoom=new Room()
}
