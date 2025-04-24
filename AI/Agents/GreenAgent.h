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
        GreenAgent(const std::shared_ptr<Entity>& entity, AIPlayroom* playroom); 
        void update() override;
       
    };


