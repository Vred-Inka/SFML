#include "gameengine.h"
#include "scenes/scenemenu.h"
#include "scenes/sceneplay.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui-SFML.h"

GameEngine::GameEngine(const std::string& path)
{
	Init(path);
}

void GameEngine::Init(const std::string& path)
{
	m_Assets.LoadFromFile(path);

	m_Window.create(sf::VideoMode(1280,768), "Kinda Mario");
	m_Window.setFramerateLimit(60);

	m_SceneMap["PLAY"] = std::make_shared<ScenePlay>(this, "assets/level1.txt");

	//ChangeScene("MENU", std::make_shared<SceneMenu>(this));
	ChangeScene("PLAY", std::make_shared<ScenePlay>(this, "assets/level1.txt"));
}

bool GameEngine::IsRunning() const 
{
	return m_Running && m_Window.isOpen();
}

void GameEngine::ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{
	m_CurrentScene = sceneName;
}

void GameEngine::Quit()
{
}

void GameEngine::Run()
{
	m_Running = true;

	while (IsRunning())
	{
		sUserInput();
		Update();
	}
}

void GameEngine::Update()
{
	GetCurrentScene()->Update();
}

void GameEngine::sUserInput()
{
	sf::Event event;
	while (m_Window.pollEvent(event))
	{
		//pass the event to imgui to be passed
		//ImGui::SFML::ProcessEvent(m_Window, event);

		if (event.type == sf::Event::Closed)
		{
			Quit();
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::X)
			{
				sf::Texture texture;
				texture.create(m_Window.getSize().x, m_Window.getSize().y);
				texture.update(m_Window);
				if (texture.copyToImage().saveToFile("test.png"))
				{
					std::cout << "screenshot saved to" << "test.png" << std::endl;
				}
			}
		}

		if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
		{
			if (GetCurrentScene()->GetActionMap().find(event.key.code) == GetCurrentScene()->GetActionMap().end())
				continue;

			const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

			GetCurrentScene()->sDoAction(Action(GetCurrentScene()->GetActionMap().at(event.key.code), actionType));			
		}
	}
}

std::shared_ptr<Scene> GameEngine::GetCurrentScene()
{
	return m_SceneMap[m_CurrentScene];
}
