#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "settings.h"

class Shape
{
protected:
	MainSettings* mMainSettings;
	sf::Text mText;

public:
	virtual void Draw(sf::RenderWindow& window) = 0;
	virtual void Move() = 0;	
	virtual void CheckCorners() = 0;
	virtual void UpdateTextPosition() = 0;

	void SetupText(std::string name, sf::Font& font);
	
};

