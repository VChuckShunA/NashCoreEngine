#pragma once
#include "Item.h"
class Coin :
    public Item
{
public:
    Coin(const std::shared_ptr<Entity>& entity);
    virtual void AddToPlayer();
    virtual void UseItem();
};

