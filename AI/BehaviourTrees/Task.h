#pragma once
#include "BehaviourTrees.h"
class Task {
protected:
   // Node* m_pNode;

public:
  //  Task(Node& node) : m_pNode(&node) {}
    virtual ~Task() {}

   // virtual Status update() = 0; // Core logic of the task.
    virtual void onInitialize() {}
  //  virtual void onTerminate(Status) {}
};