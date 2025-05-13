#pragma once
#include "../Entity.h"
#include <string>
#include "AIPlayroom.h"
class Item
{
public:
    enum Type {
        ITM_NONE,
        ITM_HEALTH,
        ITM_AMMO,
        ITM_COIN,
    };

    bool pickedUp = false;
    Item(const std::shared_ptr<Entity>& entity);
    const std::shared_ptr<Entity> entity;
    Type type;
    virtual void AddToPlayer(AIPlayroom* room) = 0;
    void HandleRemoval(AIPlayroom* room);

};

