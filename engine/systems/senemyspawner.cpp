#include "../game.h"
#include <ctime>

void Game::sEnemySpawner()
{
	if (m_IsPaused)
		return;

	if (m_EntityManager.GetEntities().size() >= m_MaxEntitiesAmount)
		return;

	if (m_CurrentFrame - m_LastEnemySpawnedTime < m_EnemyConfig.SI)
		return;

	SpawnEnemy();
}

template <typename T>
T get_random(T min, T max)
{
	static std::default_random_engine e;
	std::uniform_real_distribution<> dis(min, max + 1); // range [0, 1)
	return dis(e);
}

void Game::SpawnEnemy()
{
	SPEntity entity = m_EntityManager.AddEntity("enemy");

	float x = get_random((float)m_EnemyConfig.SR, (float)m_Window.getSize().x - m_EnemyConfig.SR);
	float y = get_random((float)m_EnemyConfig.SR, (float)m_Window.getSize().y - m_EnemyConfig.SR);
	float speed = get_random(m_EnemyConfig.SMIN, m_EnemyConfig.SMAX);
	float angel = get_random(0.0f, 360.0f);

	entity->cTransform = std::make_shared<CTransform>(
		Vec2(x, y),
		speed,
		angel);

	int v = get_random(m_EnemyConfig.VMIN, m_EnemyConfig.VMAX);
	sf::Color color(get_random(0, 255), get_random(0, 255), get_random(0, 255));
	entity->cShape = std::make_shared<CoShape>(
		m_EnemyConfig.SR,
		v,
		color,
		sf::Color(m_EnemyConfig.OR, m_EnemyConfig.OG, m_EnemyConfig.OB),
		m_EnemyConfig.OT);

	entity->cLifeSpan = std::make_shared<CLifeSpan>(m_EnemyConfig.L);
	entity->cCollision = std::make_shared<CCollision>(m_EnemyConfig.CR);
	entity->cScore = std::make_shared<CScore>(v*2);

	//record when the most recent enemy was spawned
	m_LastEnemySpawnedTime = m_CurrentFrame;
}

void Game::SpawnSmallEnemies(SPEntity& e)
{
	sf::CircleShape& circle = e->cShape->m_Circle;
	float r = circle.getRadius() / 2;

	if (r < m_MinEntityRadius || r < 0.5f)
		return;

	int v = circle.getPointCount();
	std::string tag = "enemy";
	std::string name = std::format("enemy_{:.2f}", r);

	for (size_t i = 0; i < v; i++)
	{
		SPEntity entity = m_EntityManager.AddEntity(tag);
		float angel = get_random(0.0f, 360.0f);
		angel += (360.0f / v) * i;

		float s = e->cTransform->m_Speed;
		Vec2 pos = e->cTransform->m_Pos;

		float x = round(pos.x + s * cos(DegToRad(angel)));
		float y = round(pos.y + s * sin(DegToRad(angel)));

		entity->cTransform = std::make_shared<CTransform>(
			e->cTransform->m_Pos,
			s,
			angel);

		entity->cShape = std::make_shared<CoShape>(
			r,
			v,
			circle.getFillColor(),
			circle.getFillColor(),
			circle.getOutlineThickness());

		if (e->cCollision)
		{
			entity->cCollision = std::make_shared<CCollision>(e->cCollision->m_Radius / 2);
		}

		if (e->cLifeSpan)
		{
			entity->cLifeSpan = std::make_shared<CLifeSpan>(e->cLifeSpan->m_Total);
		}

		if (e->cScore)
		{
			entity->cScore = std::make_shared<CScore>(e->cScore->m_Score * 2);
		}
	}
}
