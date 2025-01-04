#pragma once
#include <map>
#include <memory>

#include "../entitymanager.h"

#include "scene.h"

class ScenePlay : public Scene
{
	struct PlayerConfig 
	{
		float X, Y, W, H, SPEED, MAXSPEED, JUMP, GRAVITY = 0.f;
		std::string WEAPON;
	};

protected:

	std::shared_ptr<Entity> m_Player;
	std::string m_LevelPath;
	PlayerConfig m_PlayerConfig;
	bool m_DrawTextures = true;
	bool m_DrawCollision = false;
	bool m_DrawGrid = false;
	const Vec2 m_GridSize = { 64.f, 64.f };
	sf::Text m_GridText;

	void Init(const std::string& m_LevelPath);

public:
	ScenePlay(GameEngine* gameEngine, const std::string& levelPath);

	void LoadLevel(const std::string& filename);
	void LoadLevelFromConfig(const std::string& filename);

	Vec2 GridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

	void OnEnd() override;
	void Update() override;
	void sDoAction(const Action& action) override;
	void sRender() override;
	void sMovement();
	void sLifeSpan();
	void sCollision();
	void sAnimation();

	void SpawnPlayer();
	void SpawnBullet();
};

