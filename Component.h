#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "helper.h"

class CTransform
{
public:
	Vec2 m_Pos{ 0.0f, 0.0f };
	Vec2 m_Speed{ 0.0f, 0.0f };
	Vec2 m_Scale{ 1.0f, 1.0f };
	double m_Angle{0.0f};

	CTransform() {};
	CTransform(const Vec2& p, const Vec2& scale, const Vec2& speed, double a)
		: m_Pos(p), m_Scale(scale), m_Speed(speed), m_Angle(a) {};
};

class CoShape
{
public: 
	sf::CircleShape m_Circle;

	CoShape(float radius, int points, const sf::Color& fill, const sf::Color& outline, float tickness)
	:m_Circle(radius, points)
	{
		m_Circle.setFillColor(fill);
		m_Circle.setOutlineColor(outline);
		m_Circle.setOutlineThickness(tickness);
		m_Circle.setOrigin(radius, radius);
	};
};

class CCollision
{
public:
	float m_Radius{0.0f};
	CCollision(float r) 
		: m_Radius(r) {}
};

class CInput
{
public:
	bool m_Up{ false };
	bool m_Left{ false };
	bool m_Right{ false };
	bool m_Down{ false };
	bool m_Shoot{ false };

	CInput() {};
};

class CScore
{
public:
	int m_Score{ 0 };
	CScore(int r)
		: m_Score(r) {}
};

class CLifeSpan
{
public:
	int m_Remaining{ 0 };
	int m_Total{ 0 };
	CLifeSpan(int total)
		: m_Remaining(total), m_Total(total) {}
};

