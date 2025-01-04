#include "assets.h"

#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

#include <fstream>

void Assets::LoadFromFile(const std::string& path)
{
	std::ifstream ifs(path);

	if (ifs.is_open()) {
		std::string type;
		ifs >> type;
		while (!ifs.eof())
		{
			if (type == "Texture")
			{
				std::string textureName;
				std::string texturePath;
				ifs >> textureName >> texturePath;

				sf::Texture texture;
				if (!texture.loadFromFile(texturePath))
				{
					sf::Texture emptyTexture;
					emptyTexture.loadFromFile("assets/textures/misc/empty.png");

					m_TexturesMap[textureName] = std::move(emptyTexture);
				}
				else
				{
					m_TexturesMap[textureName] = std::move(texture);
				}

				ifs >> type;
			}

			if (type == "Font")
			{
				std::string fontName;
				std::string fontPath;
				ifs >> fontName >> fontPath;

				sf::Font font;
				if (!font.loadFromFile(fontPath))
				{
					std::cerr << "Could not load font!\n";
				}
				else
				{
					m_FontsMap[fontName] = std::move(font);
				}

				ifs >> type;
			}

			if (type == "Animation")
			{
				std::string animName;
				std::string animTexture;
				int frameCount;
				int animSpeed;

				ifs >> animName >> animTexture>> frameCount >> animSpeed;

				sf::Texture& t = GetTexture(animTexture);


				m_AnimationsMap[animName] =  Animation(animName, t, frameCount, animSpeed);

				ifs >> type;
			}
		
		}

	}


	ifs.close();
}

sf::Font& Assets::GetFont(const std::string& name)
{
	return m_FontsMap[name];
}

Animation& Assets::GetAnimation(const std::string& name)
{
	return m_AnimationsMap[name];
}

sf::Texture& Assets::GetTexture(const std::string& name)
{
	const std::map<std::string, sf::Texture>::iterator it = m_TexturesMap.find(name);
	if (it != m_TexturesMap.end())
	{
		return it->second;
	}
	return m_TexturesMap.begin()->second; // sample text
}

