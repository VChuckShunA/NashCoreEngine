#pragma once
#include "../BehaviourTrees/Node.h"
#include <stack>
#include <set>
#include "../House/House.h"
/*
class HouseSearchDFS : public Node
{
public:
    HouseSearchDFS(BaseAIAgent& ag);
    virtual void onInitialize() override;
    virtual void reset() override;
    Status update() override;
    std::set<int> visitedRooms;
    std::stack<std::shared_ptr<class House::Room>> roomStack;
    std::shared_ptr<class House::Room> currentRoom;
    BaseAIAgent& agent;
    bool pathGenerated = false;
};

*/