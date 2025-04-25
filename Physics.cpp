#include "Physics.h"
#include "Components.h"


Vec2 Physics::GetOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
    // return the overlap rectangle size of the bounding boxes of entity a and b
    Vec2 posA = a->getComponent<CTransform>().pos;
    Vec2 posB = b->getComponent<CTransform>().pos;
    Vec2 halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    Vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;

    Vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
    //    float overlapX = halfSizeA.x + halfSizeB.x - delta.x;
    //    float overlapY = halfSizeA.y + halfSizeB.y - delta.y;
    //
    //    return {overlapX, overlapY};
    return halfSizeA + halfSizeB - delta;
}

Vec2 Physics::GetPreviousOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b) {
    // return the previous overlap rectangle size of the bounding boxes of entity a and b
    // previous overlap uses the entity's previous position
    Vec2 posA = a->getComponent<CTransform>().prevPos;
    Vec2 posB = b->getComponent<CTransform>().prevPos;
    Vec2 halfSizeA = a->getComponent<CBoundingBox>().halfSize;
    Vec2 halfSizeB = b->getComponent<CBoundingBox>().halfSize;

    Vec2 delta{ std::abs(posA.x - posB.x), std::abs(posA.y - posB.y) };
    //    float overlapX = halfSizeA.x + halfSizeB.x - delta.x;
    //    float overlapY = halfSizeA.y + halfSizeB.y - delta.y;
    //
    //    return {overlapX, overlapY};
    return halfSizeA + halfSizeB - delta;
}

bool Physics::AABBCOllision(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b)
{
    bool AisToTheRightOfB = getLeft(a) > getRight(b);
    bool AisToTheLeftOfB = getRight(a) > getLeft(b);
    bool AisAboveB = getBottom(a) < getTop(b);
    bool AisBelowB = getTop(a) > getBottom(b);
    return !(AisToTheRightOfB
        || AisToTheLeftOfB
        || AisAboveB
        || AisBelowB);
}

float Physics::getLeft(const std::shared_ptr<Entity>& entity)
{
   return entity->getComponent<CTransform>().pos.x  - entity->getComponent<CBoundingBox>().halfSize.x;
}

float Physics::getRight(const std::shared_ptr<Entity>& entity)
{
    return entity->getComponent<CTransform>().pos.x + entity->getComponent<CBoundingBox>().halfSize.x;
}

float Physics::getTop(const std::shared_ptr<Entity>& entity)
{
    return entity->getComponent<CTransform>().pos.y + entity->getComponent<CBoundingBox>().halfSize.y;
}

float Physics::getBottom(const std::shared_ptr<Entity>& entity)
{
    return entity->getComponent<CTransform>().pos.y - entity->getComponent<CBoundingBox>().halfSize.y;
}

