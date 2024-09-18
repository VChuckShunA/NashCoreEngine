#pragma once

#include "Vec2.h"
#include <SFML/Graphics.hpp>
class Component
{
public:
	bool has = false;
};

class CTransform
{
public:
	Vec2  prevPos = { 0.0f, 0.0f };
	Vec2  pos = { 0.0f, 0.0f };
	Vec2  velocity = { 0.0f, 0.0f };
	Vec2  scale = { 1.0f, 1.0f };
	float angle = 0;

	CTransform(const Vec2 & p,const Vec2 & v, float a)
		: pos(p), velocity(v),	angle(a) {}

	CTransform()
	{
	}

	CTransform(const Vec2& p)
		: pos(p)
	{
	}

	CTransform(const Vec2& _pos, const Vec2& _velocity, const Vec2& _scale,
		float _angle)
		: pos(_pos)
		, prevPos(_pos)
		, velocity(_velocity)
		, scale(_scale)
		, angle(_angle)
	{
	}
};

class CShape
{
public:
	sf::CircleShape circle;
	CShape(float radius, int points, const sf::Color& fill,
		const sf::Color& outline, float thickness)
		: circle(radius, points)
	{
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin(radius, radius);
	}

};

class CCollision {
public:
	float collisionRadius = 0;

	CCollision(float r)
		: collisionRadius(r)
	{
	}
};

class CScore {
public:
	int score = 0;

	CScore(int s)
		: score(s)
	{
	}
};

class CLifespan {
public:
	int remaining = 0;  // amount of lifespan remaining on the entity
	int total = 0;	    // the total initial amount of lifespan

	CLifespan(int total)
		: remaining(total)
		, total(total)
	{
	}
};

class CInput {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

	CInput()
	{
	}
};

class CAnimation : public Component
{
public:
	Animation animation;
	bool repeat = false;

	CAnimation()
	{
	}

	CAnimation(const Animation& _animation, bool _repeat)
		: animation(_animation)
		, repeat(_repeat)
	{
	}
};

class CGravity : public Component
{
public:
	float gravity = 0;

	CGravity()
	{
	}

	CGravity(float g)
		: gravity(g)
	{
	}
};

class CState : public Component
{
public:
	std::string state = "None";
	CState()
	{
	}

	CState(const std::string& _state)
		: state(_state)
	{
	}
};

class CBoundingBox : public Component
{
public:
	Vec2 size;
	Vec2 halfSize;
	CBoundingBox()
	{
	}

	CBoundingBox(const Vec2& _size)
		: size(_size), halfSize(_size.x / 2.0f, _size.y / 2.0f)
	{
	}
};