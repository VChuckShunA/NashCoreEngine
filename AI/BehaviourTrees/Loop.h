#pragma once
#include "Node.h"
#include <vector>

class Loop : public Node {
public:
    Loop(Node* child) : child(child) {}
    virtual void onInitialize() override {
        child->reset();
    }
    virtual Status update() override {
        Status s = child->tick();
        if (s == BH_SUCCESS || s == BH_FAILURE) {
            child->reset();  // Reset for the next cycle.
            return BH_RUNNING;
        }
        return s;
    }
    virtual void reset() override {
        child->reset();
        m_eStatus = BH_INVALID;
    }
    virtual ~Loop() {
        delete child;
    }
private:
    Node* child;
};