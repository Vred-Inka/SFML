#include "scenemenu.h"

SceneMenu::SceneMenu(GameEngine* gameEngine)
{
}

void SceneMenu::Init()
{
	RegisterAction(sf::Keyboard::W, "UP");
	RegisterAction(sf::Keyboard::S, "DOWN");
	RegisterAction(sf::Keyboard::D, "PLAY");
	RegisterAction(sf::Keyboard::Escape, "QUIT");
}

void SceneMenu::sDoAction(const Action& action)
{
	if (action.GetType() == "START")
	{
		if (action.GetName() == "UP")
		{
			if (m_SelectedMenuIndex > 0)
			{
				m_SelectedMenuIndex--;
			}
			else
			{
				m_SelectedMenuIndex = m_MenuStrings.size() - 1;
			}
		}
		else if (action.GetName() == "DOWN")
		{
			m_SelectedMenuIndex = (m_SelectedMenuIndex + 1) % m_MenuStrings.size();
		}
		else if (action.GetName() == "PLAY")
		{
			//TODO
			//m_Game->ChangeScene("PLAY", std::make_shared<ScenePlay>(m_Game, m_LevenPaths[m_SelectedMenuIndex]));
		}
		else if (action.GetName() == "QUIT")
		{
			//TODO
			//OnEnd();
		}
	}
}

void SceneMenu::OnEnd()
{
}

void SceneMenu::Update()
{
	sRender();
}

void SceneMenu::sRender()
{
	
}
