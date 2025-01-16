#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "helper.h"
#include "animation.h"

class Component 
{
public:
	bool has = false;
};

class CTransform : public Component
{
public:
	Vec2 m_Pos{ 0.0f, 0.0f };
	Vec2 m_PrivPos{ 0.0f, 0.0f };
	Vec2 m_Scale{ 1.0f, 1.0f };
	Vec2 m_Velocity{ 0.0f, 0.0f };
	float m_Speed{ 1.0f };
	float m_Angle{0.0f};

	CTransform() {};
	CTransform(const Vec2& p) 
		: m_Pos(p)
	{};
	CTransform(const Vec2& p, float speed, float a)
		: m_Pos(p), m_Speed(speed), m_Angle(a) 
	{
		m_Velocity.x = m_Speed * cos(DegToRad(m_Angle));
		m_Velocity.y = m_Speed * sin(DegToRad(m_Angle));
	};
};

class CAnimation : public Component
{
public:
	Animation m_Animation;
	bool m_Repeat{ false };

	CAnimation() {};
	CAnimation(const Animation& animation)
		:m_Animation(animation), m_Repeat(false)
	{};
	CAnimation(const Animation& animation, bool r)
		:m_Animation(animation), m_Repeat(r)
	{
	}

};

class CGravity : public Component
{
public:
	float m_Gravity = 0.0f;

	CGravity() {};
	CGravity(float gravity) 
		:m_Gravity(gravity)
	{}
};

class CState : public Component
{
public:
	Vec2 m_PrevOverlap{ 0.f, 0.f };
	bool m_IsOnGround = false;
	bool m_IsInAir = false;
	bool m_IsRunning = false;
	std::string m_State = "jumping";
	CState() {};
	CState(const std::string& s) : m_State(s)
	{}
};

class CBoundingBox :public Component
{
public:
	Vec2 m_Size{0.0f, 0.0f};
	Vec2 m_HalfSize{0.0f, 0.0f};

	CBoundingBox() {};

	CBoundingBox(const Vec2& s)
		: m_Size(s), m_HalfSize(s.x / 2.0f, s.y / 2.0f)
	{}
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

struct Overlap
{
	size_t EntityID;
	Vec2 Overlap;
	int frame;
};

class CCollision : public Component
{

public:
	//Overlap m_PreviousFrameOverlap;

	float m_Radius{0.0f};
	bool m_CanCollide{ true };


	CCollision() {};
	CCollision(float r) 
		: m_Radius(r) {}
};

class CInput : public Component
{
public:
	bool m_Up{ false };
	bool m_Left{ false };
	bool m_Right{ false };
	bool m_Down{ false };
	bool m_Shoot{ false };
	bool m_Jump{ false };

	CInput() {};
};

class CScore : public Component
{
public:
	int m_Score{ 0 };
	CScore(){};
	CScore(int r)
		: m_Score(r) {}
};

class CLifeSpan : public Component
{
public:
	int m_Remaining{ 0 };
	int m_Total{ 0 };
	int m_LifeSpan{ 0 };
	int m_FrameCreated{0};
	CLifeSpan() {};
	CLifeSpan(int total)
		: m_Remaining(total), m_Total(total) {}
};

class CSuperPower : public Component
{
public:
	int m_Radius{ 0 };
	int m_Cooldown{ 0 };
	int m_Remaining{0};
	Vec2 m_Pos{0.0f,0.0f};
	bool m_IsActive{ false };
	int m_FadeTime{15};
	int m_RemainingFadeTime{0};

	sf::CircleShape m_Circle;

	CSuperPower(int r, int c)
		:m_Radius(r), m_Cooldown(c), m_Remaining(c), m_Circle((float)r, 15)
	{
		m_Circle.setFillColor(sf::Color(255, 255, 255, 0));
		m_Circle.setOutlineColor(sf::Color::Red);
		m_Circle.setOutlineThickness(2);
		//m_Circle.setOrigin(r, r);
	}

};

