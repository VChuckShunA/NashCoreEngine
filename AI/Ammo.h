#pragma once
#include "Item.h"
class Ammo :
    public Item
{
public:
    Ammo(const std::shared_ptr<Entity>& entity);
    virtual void AddToPlayer();
    virtual void UseItem();
};

