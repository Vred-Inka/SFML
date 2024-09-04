#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "helper.h"
#include <random>

#include "entitymanager.h"
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

struct FontConfig { std::string file; int S, R, G, B; };
struct SystemConfig { int W, H, FPS; bool FS; FontConfig Font; };

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
	SystemConfig m_SystemConfig;

	sf::Clock m_DeltaClock;
	int m_Score{0};
	int m_CurrentFrame{0};

	int m_LastEnemySpawnedTime{ 0 };

	int m_MaxEntitiesAmount = 100;

	bool m_IsPaused{ false };
	bool m_IsRunning{ true };

	void Init(const std::string& config);
	void LoadConfig(const std::string& config);
	void Update();
	void Run();

	void SetPaused(bool paused) { m_IsPaused = paused; };

	void SpawnPlayer();
	void SpawnEnemy();
	void SpawnSmallEnemies(SPEntity& e);

	void SpawnBullet(SPEntity& entity, const Vec2& target);
	void SpawnSpecialWeapon(SPEntity);

	void UpdatePlayerMove();
	void UpdateEntitiesMove();
	void UpdateEnemyMove(SPEntity& e);

	void UpdateLife(SPEntity& e);

	void DrawEntity(SPEntity& e);

	//Systes
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();
	void sLifeSpan();
	void sGUI();
	void ImguiDisplayEntities();
	void ImguiDisplayEntity(SPEntity& entity);

public:
	Game(const std::string& config);

};

