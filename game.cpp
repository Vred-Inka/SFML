#include "game.h"
#include "imgui_internal.h"
#include <math.h>
#include <format>

Game::Game(const std::string& config)
{
	Init(config);
}

void Game::Init(const std::string& path)
{
	LoadConfig(path);

	m_Window.create(sf::VideoMode(m_SystemConfig.H, m_SystemConfig.W), "Game 2.0");
	m_Window.setFramerateLimit(m_SystemConfig.FPS);

	if (!m_Font.loadFromFile(m_SystemConfig.Font.file))
	{
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	ImGui::SFML::Init(m_Window, true);

	SpawnPlayer();
	//SpawnSmallEnemies(m_Player);

	m_Text.setString("0");
	m_Text.setFont(m_Font);
	m_Text.setCharacterSize(m_SystemConfig.Font.S);
	m_Text.setFillColor(sf::Color(m_SystemConfig.Font.R, m_SystemConfig.Font.G, m_SystemConfig.Font.B));
	m_Text.setStyle(sf::Text::Bold);
	m_Text.setPosition(50.0f,50.0f);
}

void Game::Run()
{
	//TODO:  add pause functionality in here
	// some systems should function while paused (rendering)
	// some systems shouldn't (movement/input)

	while (m_IsRunning)
	{
		Update();
		m_CurrentFrame++;
	}
}

void Game::Update()
{
	m_EntityManager.Update();

	ImGui::SFML::Update(m_Window, m_DeltaClock.restart());
	ImGui::ShowDemoWindow();

	if (m_Player->cSuperPower)
	{
		if (m_Player->cSuperPower->m_Remaining > 0)
		{
			m_Player->cSuperPower->m_Remaining--;
		}

		if (m_Player->cSuperPower->m_RemainingFadeTime > 0)
		{
			m_Player->cSuperPower->m_RemainingFadeTime--;
		}
	}

	sEnemySpawner();
	sMovement();
	sUserInput();
	sCollision();
	sLifeSpan();
	sGUI();


	if (m_Player->cSuperPower)
	{
		sf::Color color = m_Player->cSuperPower->m_Remaining > 0
			? sf::Color::Blue : sf::Color::Red;
		m_Text.setFillColor(color);
	}
	m_Text.setString(std::format("Score : {}", m_Player->cScore->m_Score));

	sRender();
}

//respawn the  player in the middle of the screen
void Game::SpawnPlayer()
{
	SPEntity entity = m_EntityManager.AddEntity("player");

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(m_Window.getSize().x / 2.0f, m_Window.getSize().y / 2.0f),
		m_PlayerConfig.S,
		0.0f);

	entity->cShape = std::make_shared<CoShape>(m_PlayerConfig.SR, m_PlayerConfig.V,
		sf::Color(m_PlayerConfig.FR, m_PlayerConfig.FG, m_PlayerConfig.FB),
		sf::Color(m_PlayerConfig.OR, m_PlayerConfig.OG, m_PlayerConfig.OB),
		m_PlayerConfig.OT);

	entity->cCollision = std::make_shared<CCollision>(m_PlayerConfig.CR);

	entity->cInput = std::make_shared<CInput>();
	entity->cScore = std::make_shared<CScore>(0.0f);

	entity->cSuperPower = std::make_shared<CSuperPower>(100, m_SuperPowerCooldown);
	m_Player = entity;
}

//spawns a bullet from given entity to a target location
void Game::SpawnBullet(SPEntity& e, const Vec2& target)
{
	SPEntity entity = m_EntityManager.AddEntity("bullet");
	Vec2 pos = e->cTransform->m_Pos;

	Vec2 d{ target.x - pos.x, target.y - pos.y };
	float angel = RadToDeg(atan2f(d.y, d.x));

	entity->cTransform = std::make_shared<CTransform>(
		pos,
		m_BulletConfig.S,
		angel);

	entity->cShape = std::make_shared<CoShape>(m_BulletConfig.SR, m_BulletConfig.V,
		sf::Color(m_BulletConfig.FR, m_BulletConfig.FG, m_BulletConfig.FB),
		sf::Color(m_BulletConfig.OR, m_BulletConfig.OG, m_BulletConfig.OB),
		m_BulletConfig.OT);

	entity->cLifeSpan = std::make_shared<CLifeSpan>(m_BulletConfig.L);	
	entity->cCollision = std::make_shared<CCollision>(m_BulletConfig.CR);
}

void Game::ActivateSuperpower(const Vec2& pos)
{
	if (!m_Player->cSuperPower)
		return;

	if (m_Player->cSuperPower->m_Remaining > 0)
		return;

	m_Player->cSuperPower->m_IsActive = true;
	m_Player->cSuperPower->m_Remaining = m_Player->cSuperPower->m_Cooldown;
	m_Player->cSuperPower->m_Pos = pos;
	m_Player->cSuperPower->m_RemainingFadeTime = m_Player->cSuperPower->m_FadeTime;
}


