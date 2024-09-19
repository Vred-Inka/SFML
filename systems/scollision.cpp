#include "../game.h"

void Game::sCollision()
{
	if (!m_CollisionDetection)
		return;

	for (SPEntity& e : m_EntityManager.GetEntities("enemy"))
	{
		if (!e->IsActive())
			continue;

		CollisionWithBullet(e);
		CollisionWithPlayer(e);		
	}

	m_Player->cSuperPower->m_IsActive = false;

	/*for (SPEntity& e : m_EntityManager.GetEntities("small_enemy"))
	{
		if (!e->IsActive())
			continue;

		CollisionWithBullet(e);
		CollisionWithPlayer(e);
	}

	for (SPEntity& e : m_EntityManager.GetEntities("small_enemy_2"))
	{
		if (!e->IsActive())
			continue;

		CollisionWithBullet(e);
		CollisionWithPlayer(e);
	}
	*/
}

bool Game::CollisionWithBullet(SPEntity& entity)
{
	bool hasCollision = false;

	if (!entity->cCollision)
		return false;

	if (!entity->IsActive())
		return false;

	if (!entity->cCollision->m_CanCollide)
		return false;

	for (SPEntity& bullet : m_EntityManager.GetEntities("bullet"))
	{
		if (!bullet->IsActive())
			continue;

		if (bullet->HasCollision(*entity))
		{
			bullet->Destroy();
			entity->Destroy();

			if (entity->cScore)
			{
				m_Player->cScore->m_Score += (entity->cScore->m_Score);
			}

			SpawnSmallEnemies(entity);
			hasCollision = true;
		}
	}

	return hasCollision;
}

bool Game::CollisionWithPlayer(SPEntity& entity)
{
	bool hasCollision = false;

	if (!entity->IsActive())
		return false;

	if (m_Player->HasCollision(*entity))
	{
		entity->Destroy();
		SpawnSmallEnemies(entity);
	}

	if (m_Player->cSuperPower && m_Player->cSuperPower->m_IsActive)
	{
		if (entity->HasCollision(m_Player->cSuperPower->m_Pos, m_Player->cSuperPower->m_Radius))
		{
			entity->cTransform->m_Velocity = Vec2(0.0f, 0.0f);
			entity->cTransform->m_Speed = 0.0f;
			entity->cShape->m_Circle.setFillColor(sf::Color::Red);
			entity->cLifeSpan->m_Remaining *= 0.5;
			entity->cCollision->m_CanCollide = false;

			if (entity->cScore)
			{
				m_Player->cScore->m_Score += (entity->cScore->m_Score);
			}
		}
	}

	return hasCollision;
}


