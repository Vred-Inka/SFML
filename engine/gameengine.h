#pragma once

#include "scenes/scene.h"
#include "assets.h"

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{
protected:

	sf::RenderWindow m_Window;
	Assets m_Assets;
	std::string m_CurrentScene;
	SceneMap m_SceneMap;
	size_t m_SimulationSpeed = 1;
	bool m_Running = true;

	void Init(const std::string& path);
	void Update();

	std::shared_ptr<Scene> GetCurrentScene();

public:
	GameEngine(const std::string& path);

	void sUserInput();

	void ChangeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene = false);

	void Quit();
	void Run();

	sf::RenderWindow& GetWindow() { return m_Window; }
	Assets& GetAssets() { return m_Assets; }
	bool IsRunning() const;
};

