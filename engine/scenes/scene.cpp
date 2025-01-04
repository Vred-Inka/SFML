#include "scene.h"
#include "../gameengine.h"

Scene::Scene() 
{
}

Scene::Scene(GameEngine* gameEngine):
	m_Game(gameEngine)
{
}

void Scene::SetPaused(bool paused)
{
	m_Paused = paused;
}

void Scene::RegisterAction(int key, const std::string& action)
{
	m_ActionMap[key] = action;
}

void Scene::Simulate(const size_t frames)
{
}

size_t Scene::GetWidth() const
{
	return m_Game->GetWindow().getSize().x;
}

size_t Scene::GetHeight() const
{
	return m_Game->GetWindow().getSize().y;
}

void Scene::DrawLine(const Vec2& p1, const Vec2& p2)
{
	sf::Vertex line[] =
	{
		sf::Vertex(sf::Vector2f(p1.x, p1.y)),
		sf::Vertex(sf::Vector2f(p2.x, p2.y))
	};

	m_Game->GetWindow().draw(line, 2, sf::Lines);
}
