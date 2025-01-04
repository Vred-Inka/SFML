#pragma once
#include <map>
#include <memory>
#include <deque>

#include "scene.h"
#include "../entitymanager.h"

class SceneMenu : public Scene
{
protected:
	std::string m_Title;
	std::vector<std::string> m_MenuStrings;
	std::vector<std::string> m_LevelPath;
	sf::Text m_MenuText;
	size_t m_SelectedMenuIndex = 0;

	void Init();
	void OnEnd() override;
	void Update() override;
	void sDoAction(const Action& action) override;

public:

	SceneMenu(GameEngine* gameEngine = nullptr);

	void sRender() override;

};

