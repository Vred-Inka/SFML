#include "Shape.h"


void Shape::SetupText(std::string name, sf::Font& font)
{
	mText.setString(name);
	mText.setFont(font);
	mText.setCharacterSize(mMainSettings->mFont.size);
	mText.setFillColor(mMainSettings->mFont.GetColor());
	mText.setStyle(sf::Text::Bold);
	mText.move(0.0f, 0.0f);

	UpdateTextPosition();
}