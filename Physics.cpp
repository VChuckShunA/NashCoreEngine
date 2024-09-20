#include "Physics.h"
#include "Components.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    //check if they have bounding boxes and transform components
    
    if ((a->hasComponent<CBoundingBox>() && a->hasComponent<CTransform>()) &&
        (b->hasComponent<CBoundingBox>() && b->hasComponent<CTransform>()))
    {
        //compute and return vec2

    }
    return Vec2(0,0);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    //same as above, but use previous positions
    return Vec2(0, 0);
}
