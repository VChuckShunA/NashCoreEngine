#include "HouseSearchDFS.h"
#include "../Pathfinding/BoustrophedonPathfinder.h"
#include "../House/House.h"
#include "../Pathfinding/AStar.h"
#include "../Agents/BaseAIAgent.h"
/*


HouseSearchDFS::HouseSearchDFS(BaseAIAgent& ag):agent(ag)
{

}

void HouseSearchDFS::onInitialize()
{
    pathGenerated = false;
    agent.destinationReached = false;
    agent.currentpath.clear();
}

void HouseSearchDFS::reset()
{
}

Node::Status HouseSearchDFS::update()
{
    Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);

    currentRoom = agent.currentHouse->FindCurrentRoom(agentTile);
    if (!currentRoom) return BH_FAILURE;

    // Mark as visited if new
    if (visitedRooms.find(currentRoom.get()->roomID) == visitedRooms.end()) {
        visitedRooms.insert(currentRoom.get()->roomID);
        std::cout << "Visiting room: " << currentRoom.get()->roomID << std::endl;
    }

    // If room not fully explored, continue scanning
    if (!currentRoom.get()->searched) {

        if (!agent.currentHouse)
        {
            //      std::cout << "No Current House" << std::endl;
            return BH_FAILURE;
        }

        if (agent.currentHouse)
        {
            auto& bounds = agent.currentHouse->FindCurrentRoom(agentTile)->bounds;
            // if (!agent.currentHouse->FindCurrentRoom(agentTile)) return BH_RUNNING;
            if (agent.currentHouse->FindCurrentRoom(agentTile))
               // agent.currentHouse->PrintRoomState();

            if (!pathGenerated && agent.currentHouse->FindCurrentRoom(agentTile) != nullptr) {
                //auto coverCells = BoustrophedonPathfinder::GeneratePath(agent.currentHouse->bounds, agent.room->navmesh.navMesh);

                Vec2 agentTile = agent.room->positionToGridCordinates(agent.agent);
                auto coverCells = BoustrophedonPathfinder::GeneratePath(bounds, agent.room->navmesh.navMesh);

                std::vector<Vec2> fullPath;
                fullPath.clear();
                Vec2 last = agent.room->positionToGridCordinates(agent.agent);
                for (auto& target : coverCells) {
                    auto sub = agent.room->navmesh.FindPath(last, target);
                    if (!sub.empty()) {
                        fullPath.insert(fullPath.end(), sub.begin() + 1, sub.end());
                        last = target;
                    }
                }
                agent.currentpath = fullPath;

                pathGenerated = true;


            }


        }


        if (!agent.destinationReached)
        {
            // if (agent.currentHouse->FindCurrentRoom(agentTile)->searched) return  BH_SUCCESS;

            agent.FollowPath();
            return BH_RUNNING; //Not reached destination 
        }
    }

    // Explore unvisited connected rooms
    for (auto& door : currentRoom->doors) {
        auto nextRoom = door->connectedRoom;
        if (nextRoom && visitedRooms.find(nextRoom->roomID) == visitedRooms.end()) {
            roomStack.push(currentRoom);
            agent.destinationReached = false;
            agent.currentpath.clear();
            agent.room->navmesh.FindPath(agentTile, door->GetEntryPoint(agentTile));
            agent.FollowPath(); // triggers pathfinding or walk
            return BH_RUNNING;
        }
    }

    // Backtrack if all adjacent rooms are explored
    if (roomStack.empty()) {
        auto backtrackRoom = roomStack.top();
        roomStack.pop(); 
        agent.destinationReached = false;
        agent.currentpath.clear();
        agent.room->navmesh.FindPath(agentTile, agent.currentHouse->GetClosestMainDoor(agentTile)->GetEntryPoint(agentTile));
        agent.FollowPath(); // move to last room entry
        return BH_RUNNING;
    }

    // All rooms explored
    return BH_SUCCESS;
}
*/