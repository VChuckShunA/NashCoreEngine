#pragma once
#include "Node.h"
#include <vector>
class StatefulSequence : public Node {
public:
    void addChild(Node* child) {
        m_Children.push_back(child);
    }

    // Reset the sequence state (for example, when starting a new patrol cycle)
    virtual void reset() {
        currentIndex = 0;
        for (auto child : m_Children) {
            // Assuming your Node base class has a reset() method.
            // If not, you might need to add one.
            child->reset();
        }
    }

    // The stateful update function remembers which child to tick next.
    virtual Status update() override {
        // If we've finished all children, return success.
        if (currentIndex >= m_Children.size()) {
            return BH_SUCCESS;
        }

        // Tick the current child.
        Status s = m_Children[currentIndex]->tick();

        // If the current waypoint is reached (child returns BH_SUCCESS)
        // then advance to the next child.
        if (s == BH_SUCCESS) {
            currentIndex++;
            // If this was the last child, then the whole sequence succeeds.
            if (currentIndex >= m_Children.size())
                return BH_SUCCESS;
            else
                // Optionally, you might return BH_RUNNING here to indicate
                // that you're transitioning to the next waypoint.
                return BH_RUNNING;
        }
        // If the child is still running, just return running.
        else if (s == BH_RUNNING) {
            return BH_RUNNING;
        }
        // If the child fails, the whole sequence fails.
        else {
            return s;  // e.g., BH_FAILURE
        }
    }

    ~StatefulSequence() {
        for (Node* child : m_Children)
            delete child;
    }
private:
    std::vector<Node*> m_Children;
    size_t currentIndex;
};