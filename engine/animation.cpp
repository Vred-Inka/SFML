#include "animation.h"

Animation::Animation()
{
}

Animation::Animation(const std::string& name, sf::Texture& t)
	:Animation(name, t, 1, 0)
{
}

Animation::Animation(const std::string& name, sf::Texture& t, size_t frameCount, size_t speed, bool loop)
	:m_Name(name), m_Sprite(t), m_FrameCount(frameCount), m_CurrentFrame(0), m_Speed(speed), m_Loop(loop)
{
	m_Size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
	m_Sprite.setOrigin(m_Size.x / 2.0f, m_Size.y / 2.0f);
	m_Sprite.setTextureRect(sf::IntRect(std::floor(m_CurrentFrame) * m_Size.x, 0, m_Size. x, m_Size.y));
}

void Animation::Update()
{
	m_CurrentFrame++;

	if (m_FrameCount == 1)
		return;

	int animFrame = (m_CurrentFrame / m_Speed) % m_FrameCount;
	m_Sprite.setTextureRect(sf::IntRect(std::floor(animFrame) * m_Size.x, 0, m_Size.x, m_Size.y));

	if (animFrame == 0 && !m_Loop)
	{
		//m_Ended = true;
	}

	//TODO: 1) calculate the correct frame of animation to play based on current frame and speed
	//		2) set the texture rectangle properly  
}

bool Animation::HasEnded() const
{
	return m_Ended;
}