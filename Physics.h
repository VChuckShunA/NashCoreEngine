#pragma once
#include "vec2.h"
#include "Entity.h"
class Physics {
public:
    static Vec2 GetOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b);

    static Vec2 GetPreviousOverlap(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b);
    static bool AABBCOllision(const std::shared_ptr<Entity>& a, const std::shared_ptr<Entity>& b);
    static float getLeft(const std::shared_ptr<Entity>& entity);
    static float getRight(const std::shared_ptr<Entity>& entity);
    static float getTop(const std::shared_ptr<Entity>& entity);
    static float getBottom(const std::shared_ptr<Entity>& entity);
    static  bool GetOverlapPoint(const Vec2& point, const CTransform& boxTransform, const CBoundingBox& box);
    static Vec2 GetWallNormal(const Vec2& agentPos, const Vec2& wallPos);
};

