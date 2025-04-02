#pragma once
#include "Node.h"
#include <vector>
class Sequence : public Node {
public:
    void addChild(Node* child) {
        m_Children.push_back(child);
    }
    Status update() override {
        for (Node* child : m_Children) {
            Status status = child->tick();
            if (status != BH_SUCCESS) {
                return status;
            }
        }
        return BH_SUCCESS;
    }
private:
    std::vector<Node*> m_Children;
};