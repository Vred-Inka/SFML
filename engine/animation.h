#pragma once
#include "helper.h"

#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
	sf::Sprite m_Sprite;
	size_t m_FrameCount = 1; // total number of frames in animation
	size_t m_CurrentFrame = 0;
	size_t m_Speed = 0;
	bool m_Loop = false;
	Vec2 m_Size = { 1.f, 1.f };
	bool m_Ended = false;
	std::string m_Name = "none";

public:

	Animation();
	Animation(const std::string& name, sf::Texture& t);
	Animation(const std::string& name, sf::Texture& t, size_t frameCount, size_t speed, bool loop = false);

	void Update();
	bool HasEnded() const;
	const std::string& GetName() const { return m_Name; };
	const Vec2& GetSize() const { return m_Size; }
	sf::Sprite& GetSprite() { return m_Sprite; }
};

