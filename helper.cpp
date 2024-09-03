#include "helper.h"

Vec2::Vec2(float xin, float yin)
	:x(xin), y(yin) {}

bool Vec2::operator == (const Vec2 & rhs) const
{
	return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator != (const Vec2& rhs) const
{
	return (x != rhs.x || y != rhs.y);
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
	return Vec2(x + rhs.x, y + rhs.y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
	return Vec2(x - rhs.x, y - rhs.y);
}

Vec2 Vec2::operator * (const float val) const
{
	return Vec2(x * val, y * val);
}

Vec2 Vec2::operator / (const float val) const
{
	return Vec2(x / val, y / val);
}

void Vec2::operator += (const Vec2& rhs)
{
	x += rhs.x;
	y += rhs.y;
}

void Vec2::operator -= (const Vec2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
}

void Vec2::operator *= (const float val)
{
	x *= val;
	y *= val;
}

void Vec2::operator /= (const float val)
{
	x /= val;
	y /= val;
}

Vec2& Vec2::Add(const Vec2& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

Vec2& Vec2::Scale(float s)
{
	x *= s;
	y *= s;
	return *this;
}

Vec2& Vec2::Rotate(float deg)
{
	//x += v.x;
	//y += v.y;
	std::cout << "Vec2& Rotate(float deg) Not implemented" << std::endl;
	return *this;
}

float Vec2::Dist(const Vec2& v) const
{
	return sqrtf((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y));
}

//TODO
void Vec2::Normalize()
{

}

double Vec2::Length()
{
	return 0;//TODO
}