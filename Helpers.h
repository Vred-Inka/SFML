#pragma once
#include <iostream>

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2() {}
	Vec2(float xin, float yin)
		:x(xin), y(yin) {}

	bool operator == (const Vec2& rhs) const
	{
		return (x == rhs.x && y == rhs.y);
	}

	Vec2 operator + (const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}

	Vec2 operator * (const float val) const
	{
		return Vec2(x*val, y*val);
	}

	void operator += (const Vec2& rhs) 
	{
		x += rhs.x;
		y += rhs.y;
	}

	Vec2& Add(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vec2& Scale(float s)
	{
		x *= s;
		y *= s;
		return *this;
	}

	Vec2& Rotate(float deg)
	{
		//x += v.x;
		//y += v.y;
		std::cout << "Vec2& Rotate(float deg) Not implemented"<< std::endl;
		return *this;
	}
	
	float Dist(const Vec2& v) const
	{
		return sqrtf((v.x-x)*(v.x-x) + (v.y -y)*(v.y-y));
	}

};

