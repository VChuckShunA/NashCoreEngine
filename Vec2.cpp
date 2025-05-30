#include "Vec2.h"
#include <math.h>

Vec2::Vec2()
{
}

Vec2::Vec2(float xin, float yin)
	: x(xin)
	, y(yin)
{
}

bool Vec2::operator==(const Vec2& rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator!=(const Vec2& rhs) const
{
	return !(this->operator==(rhs));
}

Vec2 Vec2::operator+(const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator-(const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator/(const float val) const
{
	return Vec2(x / val, y / val);
}

Vec2 Vec2::operator*(const float val) const
{
	return Vec2(x * val, y * val);
}

void Vec2::operator+=(const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator-=(const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator*=(const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator/=(const float val)
{
	x /= val;
	y /= val;
}

float Vec2::dist(const Vec2& rhs) const
{
	return float(sqrt(pow(x - rhs.x, 2) + pow(y - rhs.y, 2)));
}

Vec2 Vec2::rotated(float angle) const
{
	return Vec2(
		x * cos(angle) - y * sin(angle),
		x * sin(angle) + y * cos(angle)
	);
}

float Vec2::length() const
{
	return float(sqrt(x * x + y * y));
}

float Vec2::lengthSq() const
{
	return x * x + y * y;
}

float Vec2::dot(Vec2 vector)
{
	return float((x * vector.x) + (y * vector.y));
}

void Vec2::normalize()
{
	float len = this->length();
	(*this) /= len;
}

float Vec2::cross(Vec2 vector)
{
	return x * vector.y - y * vector.x;
}
