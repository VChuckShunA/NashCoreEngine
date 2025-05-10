#pragma once
#include "Item.h"
#include "AIPlayroom.h"
class Coin :
    public Item
{
public:
    Coin(const std::shared_ptr<Entity>& entity);
    virtual void AddToPlayer(AIPlayroom* room) override;
};

