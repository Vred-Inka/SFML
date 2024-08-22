#pragma once

#include <SFML/Graphics.hpp>
#include "Shape.h"

class RShape : public Shape
{
	RectangleSettings mSettings;
	sf::RectangleShape mShape;

public:
	RShape(RectangleSettings& rs, MainSettings& ms, sf::Font& font);

	void Draw(sf::RenderWindow& window) override;
	void Move() override;

	void UpdateTextPosition() override;

	void CheckCorners();
};