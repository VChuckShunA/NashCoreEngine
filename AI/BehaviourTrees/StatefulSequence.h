#pragma once
#include "Node.h"
#include <vector>
class StatefulSequence : public Node {
public:
    void addChild(Node* child) {
        m_Children.emplace_back(std::move(child));
    }

    // Reset the sequence state when starting a new patrol cycle
    virtual void reset() {
        currentIndex = 0;
        for (const auto& child : m_Children) {
            child->reset();
        }
        m_eStatus = BH_INVALID;
    }

    // The stateful update function remembers which child to tick next.
    virtual Status update() override {
        // If we've finished all children, return success.
        if (currentIndex >= m_Children.size()) {
            reset();
            return BH_RUNNING;
            //NOTE: This used to return running, and Reset wasn't called here
        }

        // Tick the current child.
        Status s = m_Children[currentIndex]->tick();

        // If the current child returns BH_SUCCESS
        // then advance to the next child.
        if (s == BH_SUCCESS) {
            currentIndex++;
            // If this was the last child, then the whole sequence succeeds.
            if (currentIndex >= m_Children.size())
                return BH_SUCCESS;
            else
                // return BH_RUNNING here to indicate
                return BH_RUNNING;
        }
        // If the child is still running, just return running.
        else if (s == BH_RUNNING) {
            return BH_RUNNING;
        }
        // If the child fails, the whole sequence fails.
        else {
            return s;
        }
    }
    size_t getCurrentChildIndex() const {
        return currentIndex;
    }

    Node* getCurrentChild() const {
        
            return m_Children[currentIndex].get();
        
    }
    StatefulSequence(){  }
    ~StatefulSequence() = default;
private:
    std::vector<std::unique_ptr<Node>>  m_Children;
    size_t currentIndex=0;
};