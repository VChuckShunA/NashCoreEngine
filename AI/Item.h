#pragma once
class Item
{
public:
    enum Type {
        ITM_NONE,
        ITM_HEALTH,
        ITM_AMMO,
        ITM_COIN,
    };

    Type type;
    virtual void AddToPlayer() = 0;
    virtual void UseItem() = 0;

};

