#include "House.h"

Vec2 House::Door::GetEntryPoint(Vec2 position)
{
    //Compare entry points with position and get the furthest point

    float dist1 = (entryPoint1 - position).length();  // If you have a Vec2::length() function
    float dist2 = (entryPoint2 - position).length();

    if (dist1 > dist2)
        return entryPoint1;
    else
        return entryPoint2;
}

void House::Door::ResetDoor()
{
	scanned = false;
}


House::Door* House::GetClosestMainDoor(Vec2& agentPosition)
{
    if (mainDoors.empty()) return nullptr; // No doors

    float minDistSq = std::numeric_limits<float>::max();
    Door* closestDoor=nullptr;

    for (const auto& door : mainDoors)
    {
        float distSq = (agentPosition - door->doorPosition).lengthSq();
        if (distSq < minDistSq)
        {
            minDistSq = distSq;
            closestDoor = door.get();
        }
    }

    return closestDoor;
}

void House::AddMainDoor(Vec2 mainDoorCords,bool horizonal)
{
    auto door = std::make_shared<Door>();
	door->doorPosition = mainDoorCords;
    if (horizonal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(1, 0);
        door->entryPoint2 = mainDoorCords + Vec2(1, 0); // example offset, adjust to your logic
    }
    if (!horizonal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(0, 1);
        door->entryPoint2 = mainDoorCords + Vec2(0, 1); // example offset, adjust to your logic
    }

    mainDoors.push_back(door);
}



void House::AddRoom(int top, int left, int width, int height,  House* house, int roomID)
{
    auto newRoom = std::make_shared<House::Room>();
    newRoom->bounds = sf::IntRect(left, top, width, height);
    newRoom->roomID = roomID;
   // newRoom->doors = doors;
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

House::Door* House::Room::GetClosestRoomDoor(Vec2 position)
{
    if (doors.empty()) return nullptr; // No doors

    float minDistSq = std::numeric_limits<float>::max();
    Door* closestDoor = nullptr;

    for (const auto& door : doors)
    {
        float distSq = (position - door->doorPosition).lengthSq();
        if (distSq < minDistSq)
        {
            if (!door.get()->connectedRoom.get()->searched)
            {
                minDistSq = distSq;
                closestDoor = door.get();
            }
        }
    }

    return closestDoor;
}

void House::Room::AddRoomDoor(Vec2 mainDoorCords, bool horizontal)
{
    auto door = std::make_shared<Door>();
    door->doorPosition = mainDoorCords;
    if (horizontal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(1, 0);
        door->entryPoint2 = mainDoorCords + Vec2(1, 0); // example offset, adjust to your logic
    }
    if (!horizontal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(0, 1);
        door->entryPoint2 = mainDoorCords + Vec2(0, 1); // example offset, adjust to your logic
    }

    doors.push_back(door);
}
