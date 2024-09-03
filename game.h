#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "helper.h"

#include "entitymanager.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
public:
	sf::RenderWindow m_Window;
	EntityManager m_EntityManager;
	SPEntity m_Player;
	sf::Font m_Font;
	sf::Text m_Text;

	PlayerConfig m_PlayerConfig;
	EnemyConfig m_EnemyConfig;
	BulletConfig m_BulletConfig;

	sf::Clock m_DeltaClock;
	int m_Score{0};
	int m_CurrentFrame{0};

	int m_LastEnemySpawnedTime{ 0 };

	bool m_IsPaused{ false };
	bool m_IsRunning{ true };

	void Init(const std::string& config);
	void Update();
	void Run();

	void SetPaused(bool paused) { m_IsPaused = paused; };

	void SpawnPlayer();
	void SpawnEnemy();
	void SpawnSmallEnemies(SPEntity e);

	void SpawnBullet(SPEntity& entity, const Vec2& target);
	void SpawnSpecialWeapon(SPEntity);

	//Systes
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sLifeSpan();
	void sGUI();

public:
	Game(const std::string& config);

};

