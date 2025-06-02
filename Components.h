#pragma once

#include "Animation.h"
#include "Vec2.h"
#include <SFML/Graphics.hpp>
#include<numbers>
#include <iostream>
class Entity;
class Item;
class Component {
public:
    bool has = false;
};

class CTransform : public Component {
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = { 0.0, 0.0 };
    Vec2 velocity = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    float angle = 0;

    CTransform() = default;

    explicit CTransform(const Vec2& p) : pos(p) {}

    CTransform(const Vec2& p, const Vec2& speed, const Vec2& s, float a)
        : pos(p), prevPos(p), velocity(speed), scale(s), angle(a) {}
};

class CLifespan : public Component {
public:
    int lifespan = 0;
    int frameCreated = 0;

    CLifespan() = default;

    explicit CLifespan(int duration, int frame)
        : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component {
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool canShoot = true;
    bool canJump = true;

    CInput() = default;
};

class CBoundingBox : public Component {
public:
    Vec2 size;
    Vec2 halfSize;

    CBoundingBox() = default;

    explicit CBoundingBox(const Vec2& s) : size(s), halfSize(s.x / 2.0f, s.y / 2.0f) {}
};

class CAnimation : public Component {
public:
    Animation animation;
    bool repeat = false;

    CAnimation() = default;

    CAnimation(Animation a, bool r) : animation(std::move(a)), repeat(r) {}
};

class CGravity : public Component {
public:
    float gravity = 0;

    CGravity() = default;

    explicit CGravity(float g) : gravity(g) {}
};

class CState : public Component {
public:
    std::string state = "jumping";
    std::string previousState = "jumping";
    bool changeAnimation = false;

    CState() = default;

    explicit CState(std::string s) : state(std::move(s)) {}
};

class CVision : public Component {
public:
    float fovAngle;    // Field of View in degrees
    float visionRange; // Max sight distance
    bool seesPlayer=false;   // Detection flags
    bool seesFood=false;
    bool seesAmmo=false;
    bool seesCoin = false;
    bool seesWall = false;
    std::shared_ptr<Entity> Target;
    std::vector<std::shared_ptr<Entity>> visibleBricks;
    std::shared_ptr<Item> Item;
    std::shared_ptr<Entity> NearestBrick;
    std::shared_ptr<Entity> LastKnownBrick;
    CVision(float fov = 90.0f, float range = 300.0f)
        : fovAngle(fov), visionRange(range), seesPlayer(false) {
    }

    bool IsTargetInFOV(const Vec2& eyePos,
        Vec2& lookDir,      // must be normalized!
        const Vec2& targetPos) const
    {
        Vec2 toTarget = targetPos - eyePos;
        float dist = toTarget.length();
        if (dist > visionRange)
            return false;
        toTarget.normalize();
        Vec2 dir = toTarget;
        float cosHalf = std::cos((fovAngle * 0.5f) * (std::numbers::pi / 180.0f));
       
        float dp = lookDir.dot(dir);;
        return dp >= cosHalf;
    }
};



class CWallTracker : public Component
{

public:
    bool wallRight = false;
    bool wallLeft = false;

    float traceDistance = 300.0f;    // Ray length
    int   numSideRays = 3;          // Number of rays in fan
    float sideArcAngle = 45.0f;     // Arc in degrees

    bool  tracingRight = true;      // Right-hand or left-hand rule
    bool  hasValidHit = false;      // Did any rays hit a wall?
    Vec2 lastHitNormalRight;        //Normal of Right rays
    Vec2 lastHitNormalLeft;         //Normal of Left rays

    // Stores last frame’s ray directions
    std::vector<Vec2> lastRightRayDirs;
    std::vector<Vec2> lastLeftRayDirs;

    CWallTracker() = default;

    // Draw the side rays in red
 /*   void draw(sf::RenderWindow& window, const Vec2& pos) {
        for (auto& dir : lastRayDirs) {
            Vec2 end = pos + dir * traceDistance;
            sf::Vertex line[2] = {
                { sf::Vector2f(pos.x, pos.y), sf::Color::Red },
                { sf::Vector2f(end.x, end.y), sf::Color::Red }
            };
            std::cout << "Drawing Lines" << std::endl;
            window.draw(line, 30, sf::Lines);
        }
    }*/

    //if (hasValidHit) {
    //    // 3a) We found a wall on our tracing side: “hug” it.
    //    lastHitNormal = bestNormal;
    //    loseTimer = 0.0f;

    //    // Move forward alongside the wall
    //    Vec2 moveDir = forward * forwardSpeed * Time::deltaTime;
    //    t.pos += moveDir;

    //    // (Optionally, you can also nudge slightly away from the wall to maintain a tiny offset:
    //    //   Vec2 away = bestNormal * 2.0f; 
    //    //   t.pos += away; 
    //    // )
    //}
    //else {
    //    // 3b) No wall detected on that side → we’re “losing” the wall
    //    loseTimer += Time::deltaTime;

    //    // If we’ve lost it for too long, switch tracing direction (flip handedness).
    //    if (loseTimer > 0.5f) {
    //        tracingRight = !tracingRight;
    //        loseTimer = 0.0f;
    //        return;
    //    }

    //    // Otherwise, gently rotate toward the side to try to reacquire:
    //    float turn = turnSpeed * (tracingRight ? +1.0f : -1.0f);
    //    t.angle += turn;

    //    // Also step forward a little so we don’t just spin in place:
    //    Vec2 moveDir = forward * (forwardSpeed * 0.3f) * Time::deltaTime;
    //    t.pos += moveDir;
    //}
//}
};