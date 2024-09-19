#pragma once
#include <iostream>

class Vec2
{
public:
	float x{0.0f};
	float y{0.0f};

	Vec2(float xin, float yin);

	bool operator == (const Vec2& rhs) const;
	bool operator != (const Vec2& rhs) const;

	Vec2 operator + (const Vec2& rhs) const;	
	Vec2 operator - (const Vec2& rhs) const;
	Vec2 operator * (const float val) const;
	Vec2 operator / (const float val) const;	

	void operator += (const Vec2& rhs);
	void operator -= (const Vec2& rhs);
	void operator *= (const float val);
	void operator /= (const float val);
	
	Vec2& Add(const Vec2& v);

	Vec2& Scale(float s);
	
	Vec2& Rotate(float deg);
	float Dist(const Vec2& v) const;

	void Normalize();
	double Length();
};

static const float PI = 3.1416f;

static float DegToRad(float degrees) {
	return degrees * (PI / 180.0f);
}

static float RadToDeg(float rad) {
	return (rad * 180.0f) / PI;
}

