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

