#pragma once
#include "Node.h"
#include <vector>
class Sequence : public Node {
public:
    Sequence(){ Name = "Sequence"; }
    void addChild(Node* child) {
        m_Children.emplace_back(std::move(child));
    }
    Status update() override {
        for (const auto& child : m_Children) {
            Status status = child->tick();
            if (status != BH_SUCCESS) {
                return status;
            }
        }
        return BH_SUCCESS;
    }
    ~Sequence() = default;
private:
    std::vector<std::unique_ptr<Node>>  m_Children;
};