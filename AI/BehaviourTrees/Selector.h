#pragma once
#include "Node.h"
#include <vector>
class Selector : public Node {
public:
    Selector(){  }
    void addChild(Node* child) {
        m_Children.emplace_back(std::move(child));
    }
    Status update() override {
        for (const auto& child : m_Children) {
            Status status = child->tick();
            if (status != BH_FAILURE) {
                return status;
            }
        }
        return BH_FAILURE;
    }
    ~Selector() = default;
private:
    std::vector<std::unique_ptr<Node>>  m_Children;
};