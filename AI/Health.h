#pragma once
#include "Item.h"
class Health :public Item
{
public:
    Health(const std::shared_ptr<Entity>& entity);

    virtual void AddToPlayer();
    virtual void UseItem();
};

