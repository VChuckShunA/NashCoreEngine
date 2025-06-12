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

void House::AddRoom(int top, int left, int width, int height, std::vector<std::shared_ptr<Door>> doors, House* house, int roomID)
{
    auto newRoom = std::make_shared<House::Room>();
    newRoom->bounds = sf::IntRect(left, top, width, height);
    newRoom->roomID = roomID;
    newRoom->doors = doors;
    house->rooms.push_back(newRoom);
}

void House::AddBounds(int top, int left, int width, int height)
{
    bounds= sf::IntRect(left, top, width, height);
}

std::shared_ptr<House::Room> House::FindCurrentRoom(Vec2 agentPosition)
{
    int gridX = int(std::floor(agentPosition.x));
    int gridY = int(std::floor(agentPosition.y));

    std::cout << "Floored : " << gridX <<" , "<< gridY << std::endl;
    for (auto& room : rooms)
    {
       if((agentPosition.x>= room->bounds.left && agentPosition.x <= room->bounds.left +room->bounds.width) &&
           (agentPosition.y <= room->bounds.top && agentPosition.y >= room->bounds.top - room->bounds.height)
           )
        {
            std::cout << "Found Room" << std::endl;
            return room;
        }
    }
    std::cout << "DID NOT FIND ROOM " << std::endl;
    return nullptr; // not found
}

bool House::IsBoundsSet()
{
    return bounds.width > 0 && bounds.height > 0;
}
