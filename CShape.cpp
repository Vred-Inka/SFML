#include "CShape.h"

CShape::CShape(CircleSettings& cs, MainSettings& ms, sf::Font& font)
{
	mMainSettings = &ms;
	mSettings = cs;
	mShape.setRadius(mSettings.GetRadius());
	mShape.setFillColor(mSettings.GetColor());
	mShape.setPosition(mSettings.GetX(), mSettings.GetY());
	SetupText(mSettings.GetName(), font);
}

void CShape::Draw(sf::RenderWindow& window)
{
	window.draw(mShape);
	window.draw(mText);
}

void CShape::Move()
{
	CheckCorners();

	mShape.move(mSettings.GetSpeedX(), mSettings.GetSpeedY());
	UpdateTextPosition();
}

void CShape::UpdateTextPosition()
{
	float left = mShape.getPosition().x;
	float top = mShape.getPosition().y;

	float textWidth = mText.getGlobalBounds().width;
	float textHeight = mText.getLocalBounds().height;

	float x = left + (mShape.getLocalBounds().width * 0.5f) - (textWidth * 0.5f);
	float y = top + (mShape.getLocalBounds().height * 0.5f) - (textHeight * 0.5f);

	mText.setPosition(x, y);
}

void CShape::CheckCorners()
{
	float left = mShape.getPosition().x;
	float right = mShape.getPosition().x + mShape.getLocalBounds().width;
	float top = mShape.getPosition().y;
	float botton = mShape.getPosition().y + mShape.getLocalBounds().height;


	if (left <= 0 || right >= mMainSettings->GetWindowWidth())
	{
		mSettings.SetSpeedX(mSettings.GetSpeedX() * -1.0f);
	}

	if (top <= 0 || botton >= mMainSettings->GetWindowHeight())
	{
		mSettings.SetSpeedY(mSettings.GetSpeedY() * -1.0f);
	}
}

