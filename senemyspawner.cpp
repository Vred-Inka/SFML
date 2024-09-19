#include "game.h"
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

	int x = get_random(m_EnemyConfig.SR, (int)m_Window.getSize().x - m_EnemyConfig.SR);
	int y = get_random(m_EnemyConfig.SR, (int)m_Window.getSize().y - m_EnemyConfig.SR);
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

	//record when the most recent enemy was spawned
	m_LastEnemySpawnedTime = m_CurrentFrame;
}

// spawn the small enemies when a big one(input entity e) explodes
void Game::SpawnSmallEnemies(SPEntity & e)
{
	if (e->GetTag() == "small_enemy_2")
		return;

	int v = e->cShape->m_Circle.getPointCount();
	sf::CircleShape& circle = e->cShape->m_Circle;
	std::string tag = e->GetTag() == "small_enemy" ? "small_enemy_2" : "small_enemy";

	for (size_t i = 0; i < v; i++)
	{

		SPEntity entity = m_EntityManager.AddEntity(tag);
		float angel = (360.0f / v) * i;

		float s = e->cTransform->m_Speed;
		Vec2 pos = e->cTransform->m_Pos;

		float x = round(pos.x + s * cos(DegToRad(angel)));
		float y = round(pos.y + s * sin(DegToRad(angel)));

		entity->cTransform = std::make_shared<CTransform>(
			Vec2(x, y),
			s,
			angel);

		entity->cShape = std::make_shared<CoShape>(
			circle.getRadius() / 2,
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
	}

	//when we create the smaller enemy, we have to read the values of the original
	// - spawn a number of small enemies equal to the vertices of the original enemy
	// - set each small enemy to the same color as the original, half the size
	// - small enemies are worth double points of the original enemy
}

//spawns a bullet from given entity to a target location
void Game::SpawnBullet(SPEntity& entity, const Vec2& target)
{
	//TODO: implement the spawning of a bullet which travels toward target
	//	- bullet speed is given as a scalar speed
	//	- you must set the velocity by using formula in notes
}

void Game::SpawnSpecialWeapon(SPEntity)
{
	//TODO: implement
}