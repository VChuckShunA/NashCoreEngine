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
