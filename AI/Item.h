#pragma once
#include "../Entity.h"
#include <string>
class Item
{
public:
    enum Type {
        ITM_NONE,
        ITM_HEALTH,
        ITM_AMMO,
        ITM_COIN,
    };
    Item(const std::shared_ptr<Entity>& entity);
    const std::shared_ptr<Entity>& entity;
    Type type;
    virtual void AddToPlayer() = 0;
    virtual void UseItem() = 0;

};

