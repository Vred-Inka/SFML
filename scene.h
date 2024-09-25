#pragma once
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "action.h"

class Scene
{
	std::map <int, std::string> mActionMap;
public:
	Scene() {};

	virtual void sDoAction(const Action& action) = 0;

	void RegisterAction(int key, const std::string& action);
};

class SceneMenu : public Scene
{
	unsigned int m_SelectedMenuIndex = 0;
	std::vector<std::string> m_MenuStrings;

public:

	void Init();

	virtual void sDoAction(const Action& action);
};

