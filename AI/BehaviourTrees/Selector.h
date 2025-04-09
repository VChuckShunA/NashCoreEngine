#pragma once
#include "Node.h"
#include <vector>
class Selector : public Node {
public:
    void addChild(Node* child) {
        m_Children.push_back(child);
    }
    Status update() override {
        for (Node* child : m_Children) {
            Status status = child->tick();
            if (status != BH_FAILURE) {
                return status;
            }
        }
        return BH_FAILURE;
    }
    ~Selector() {
        for (Node* child : m_Children)
            delete child;
    }
private:
    std::vector<Node*> m_Children;
};