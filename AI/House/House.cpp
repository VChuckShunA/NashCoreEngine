#include "House.h"

void House::Door::ResetDoor()
{
	scanned = false;
}


Vec2 House::GetClosestMainDoor(Vec2& agentPosition)
{
    if (mainDoors.empty()) return Vec2(); // No doors

    float minDistSq = std::numeric_limits<float>::max();
    Vec2 closestDoor;

    for (const auto& door : mainDoors)
    {
        float distSq = (agentPosition - door->doorPosition).lengthSq();
        if (distSq < minDistSq)
        {
            minDistSq = distSq;
            closestDoor = door->doorPosition;
        }
    }

    return closestDoor;
}

void House::AddMainDoor(Vec2 mainDoorCords)
{
    auto door = std::make_shared<Door>();
	door->doorPosition = mainDoorCords;
   // door->entryPoint1 = mainDoorCords;
  //  door->entryPoint2 = mainDoorCords + Vec2(1, 0); // example offset, adjust to your logic

    mainDoors.push_back(door);
}

void House::AddRoom(Vec2& topleft, Vec2& topRight, Vec2& Bottomleft, Vec2& BottomRight, std::vector<std::shared_ptr<Door>> doors)
{
	//Room* newRoom=new Room()
}
