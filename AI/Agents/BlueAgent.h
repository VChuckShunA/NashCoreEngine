#pragma once
#include <vector>
#include <iostream>
#include "../BehaviourTrees/Node.h"
#include "../BehaviourTrees/Selector.h"
#include "../BehaviourTrees/Sequence.h"
#include "../BehaviourTrees/StatefulSequence.h"
#include "../BehaviourTrees/Loop.h"
#include "../../EntityManager.h"
#include "BaseAIAgent.h"
#include <SFML/System.hpp>



class BlueAgent : public BaseAIAgent
{

public:
    BlueAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom);
    void update() override;
    const int getHealth();
    bool hasAmmo() override;
    bool hasFood() override;
    bool needsAmmo() override;
    bool needsFood() override;
};


