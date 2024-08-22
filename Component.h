#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Helpers.h"

class Component
{
};

class CTransform
{
public:
	Vec2 pos = { 0,0 };
	Vec2 velocity = { 0,0 };

	CTransform() {};
	CTransform(const Vec2& p, const Vec2& v)
		: pos(p), velocity(v) {};
};

class CName
{
	std::string mName;
public:
	CName() {};
	CName(std::string name)
		:mName(name) {};
};

class CoShape
{
public: 
	sf::CircleShape shape;
	CoShape() {};
};

class CBBox
{

};

