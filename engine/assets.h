#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>

#include "animation.h"

class Assets
{
	std::map<std::string, sf::Font> m_FontsMap;
	std::map<std::string, Animation> m_AnimationsMap;
	std::map<std::string, sf::Texture> m_TexturesMap;

public:
	void LoadFromFile(const std::string& name);
	sf::Font& GetFont(const std::string& name);	
    Animation& GetAnimation(const std::string& name);
    sf::Texture& GetTexture(const std::string& name);
};

