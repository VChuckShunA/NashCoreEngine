#include "House.h"
#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>
Vec2 House::Door::GetEntryPoint(Vec2 position)
{
    //Compare entry points with position and get the furthest point
    float dist1 = (entryPoint1 - position).length();  // If you have a Vec2::length() function
    float dist2 = (entryPoint2 - position).length();

    if (dist1 > dist2)
    {
        std::cout << "Entry Point 1 : " << entryPoint1.x << " , " << entryPoint1.y << std::endl;
        return entryPoint1;
    }
    else
    {
        std::cout << "Entry Point 2 : " << entryPoint2.x << " , " << entryPoint2.y << std::endl;
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
    newRoom->bounds = sf::FloatRect(left, top, width, height);
    newRoom->roomID = roomID;
   // newRoom->doors = doors;
    house->rooms.push_back(newRoom);
}

void House::AddBounds(int top, int left, int width, int height)
{
    bounds= sf::FloatRect(left, top, width, height);
}

std::shared_ptr<House::Room> House::FindCurrentRoom(Vec2 agentPosition)
{
 
    for (auto& room : rooms)
    {
        std::cout << "Room ID "<< room->roomID << std::endl;
        std::cout << "Top " << room->bounds.top << std::endl;
        std::cout << "Left " << room->bounds.left << std::endl;
        std::cout << "Width " << room->bounds.width << std::endl;
        std::cout << "Height " << room->bounds.height << std::endl;
        std::cout << "Bottom " << room->bounds.top - room->bounds.height << std::endl;
        std::cout << "Agent Position " << agentPosition.x<<" , "<< agentPosition.y << std::endl;
        bool insideX = agentPosition.x >= room->bounds.left && agentPosition.x <= room->bounds.left + room->bounds.width;
        bool insideY = agentPosition.y <= room->bounds.top && agentPosition.y+1 >= room->bounds.top - room->bounds.height;


        std::cout << "insideX " << insideX << std::endl;
        std::cout << "insideY " << insideY << std::endl;
       if((insideX) &&
           (insideY))
        {
            std::cout << "Found Room" << std::endl;
            return room;
        }
    }
  //  std::cout << "DID NOT FIND ROOM " << std::endl;
    return nullptr; // not found
}

bool House::IsBoundsSet()
{
    return bounds.width > 0 && bounds.height > 0;
}

House::Door* House::Room::GetClosestRoomDoor(Vec2 position)
{
    std::cout << "Starting Closest Dooor " << std::endl;
    if (doors.empty()) return nullptr; // No doors

    float minDistSq = std::numeric_limits<float>::max();
    Door* closestDoor = nullptr;

    for (const auto& door : doors)
    {
        std::cout << "Ierating doors" << std::endl;
        float distSq = (position - door->doorPosition).lengthSq();
        if (distSq < minDistSq)
        {
            std::cout << "Ierating doors2" << std::endl;
            std::cout << (door.get()) << std::endl;
            std::cout << (door.get()->connectedRoom.get()) << std::endl;
            if (!door.get()->connectedRoom.get()->searched)
            {
                std::cout << "Ierating doors3" << std::endl;
                minDistSq = distSq;
                closestDoor = door.get();
            }
        }
    }
    std::cout << "Closest Dooor " << std::endl;
    std::cout << "Closest Dooor " << closestDoor->doorPosition.x << " , " << closestDoor->doorPosition.y << std::endl;
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
    std::cout << "Added room door " << roomID << std::endl;
    doors.push_back(door);
}
