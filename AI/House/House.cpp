#include "House.h"
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
Vec2 House::Door::GetEntryPoint(Vec2 position)
{
    //Compare entry points with position and get the furthest point
    float dist1 = (entryPoint1 - position).length();
    float dist2 = (entryPoint2 - position).length();

    if (dist1 > dist2)
    {
        return entryPoint1;
    }
    else
    {
        return entryPoint2;
    }
}

void House::Door::ResetDoor()
{
	scanned = false;
}


House::Door* House::GetClosestMainDoor(Vec2& agentPosition)
{
    if (mainDoors.empty()) return nullptr; // No doors

    float minDistSq = std::numeric_limits<float>::max();
    Door* closestDoor = nullptr;

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
        door->entryPoint2 = mainDoorCords + Vec2(1, 0);
    }
    if (!horizonal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(0, 1);
        door->entryPoint2 = mainDoorCords + Vec2(0, 1);
    }

    mainDoors.push_back(door);
}



std::shared_ptr<House::Room> House::AddRoom(int top, int left, int width, int height,  House* house, int roomID)
{
    auto newRoom = std::make_shared<House::Room>();
    newRoom->bounds = sf::FloatRect(left, top, width, height);
    newRoom->roomID = roomID;
   // newRoom->doors = doors;
    house->rooms.push_back(newRoom);
    return newRoom;
}

void House::AddBounds(int top, int left, int width, int height)
{
    bounds= sf::FloatRect(left, top, width, height);
}

std::shared_ptr<House::Room> House::FindCurrentRoom(Vec2 agentPosition)
{
    int cellX = static_cast<int>(std::floor(agentPosition.x));
    int cellY = static_cast<int>(std::floor(agentPosition.y));

 
    for (auto& room : rooms)
    {
      
        bool insideX = cellX >= room->bounds.left && cellX <= room->bounds.left + room->bounds.width;
        bool insideY = cellY <= room->bounds.top && cellY >= room->bounds.top - room->bounds.height;

       if((insideX) &&
           (insideY))
        {
            std::cout << "Found Room "<<room->roomID << std::endl;
            std::cout << "Agent Position " << cellX <<" , "<< cellY << std::endl;
            std::cout << "room->bounds.left " << room->bounds.left  << std::endl;
            std::cout << "room->bounds.left + room->bounds.width " << room->bounds.left + room->bounds.width << std::endl;
            std::cout << "room->bounds.top " << room->bounds.top << std::endl;
            std::cout << "room->bounds.top - room->bounds.height " << room->bounds.top - room->bounds.height << std::endl;
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

    //float minDistSq = std::numeric_limits<float>::max();
    Door* closestDoor = nullptr;

    for (const auto& door : doors)
    {
      //  float distSq = (position - door->doorPosition).lengthSq();
       // if (distSq < minDistSq)
        {
          // std::cout << (door.get()) << std::endl;
          //  std::cout << (door.get()->connectedRoom.get()) << std::endl;
            if (!door.get()->connectedRoom.get()->searched)
            {
                //minDistSq = distSq;
                closestDoor = door.get();
            }
        }
    }
    return closestDoor;
}

std::shared_ptr<House::Door> House::Room::AddRoomDoor(Vec2 mainDoorCords, bool horizontal)
{
    auto door = std::make_shared<Door>();
    door->doorPosition = mainDoorCords;
    if (horizontal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(1, 0);
        door->entryPoint2 = mainDoorCords + Vec2(1, 0); 
    }
    if (!horizontal)
    {
        door->entryPoint1 = mainDoorCords - Vec2(0, 1);
        door->entryPoint2 = mainDoorCords + Vec2(0, 1); 
    }
    doors.push_back(door);

    return door;
}
