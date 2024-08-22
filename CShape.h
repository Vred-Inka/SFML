#pragma once

#include "Shape.h"

class CShape : public Shape
{
	CircleSettings mSettings;
	sf::CircleShape mShape;

public:
	CShape(CircleSettings& cs, MainSettings& ms, sf::Font& font);
	void Draw(sf::RenderWindow& window) override;
	void Move() override;

	void UpdateTextPosition() override;

	void CheckCorners();


};

