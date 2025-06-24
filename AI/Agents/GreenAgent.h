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


  
    class GreenAgent : public BaseAIAgent
    {
        
    public:
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom,Vec2 waypoint1, Vec2 waypoint2, Vec2 waypoint3);
        void update() override;
        void HandleDeath() override;
    };


