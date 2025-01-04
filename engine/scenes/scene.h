#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <map>
#include <memory>

#include "../action.h"
#include "../entitymanager.h"
#include "../helper.h"

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
	GameEngine* m_Game = nullptr;
	EntityManager m_EntityManager;
	ActionMap m_ActionMap;
	bool m_Paused = false;
	bool m_HasEnded = false;
	size_t m_CurrentFrame = 0;

	virtual void OnEnd() = 0;
	void SetPaused(bool paused);

public:
	Scene();
	Scene(GameEngine* gameEngine);

	virtual void Update() = 0;
	virtual void sDoAction(const Action& action) = 0;
	virtual void sRender() = 0;

	void Simulate(const size_t frames);
	void RegisterAction(int key, const std::string& action);

	size_t GetWidth() const;
	size_t GetHeight() const;
	size_t GetCurrentFrame() const { return m_CurrentFrame; }

	bool HasEnded() const { return m_HasEnded; }
	const ActionMap& GetActionMap() const { return m_ActionMap; }
	void DrawLine(const Vec2& p1, const Vec2& p2);
};
