#include "../game.h"

void Game::sMovement()
{
	UpdatePlayerMove();
	UpdateEntitiesMove();
}

void Game::UpdatePlayerMove()
{
	if (!m_Player->cShape)
		return;

	sf::CircleShape& circle = m_Player->cShape->m_Circle;

	float left = circle.getPosition().x - circle.getRadius();
	float right = circle.getPosition().x + circle.getRadius();
	float top = circle.getPosition().y - circle.getRadius();
	float botton = circle.getPosition().y + circle.getRadius();

	if (m_Player->cInput->m_Right && right + m_Player->cTransform->m_Speed < m_Window.getSize().x)
	{
		m_Player->cTransform->m_Pos.x += m_Player->cTransform->m_Speed;
	}

	if (m_Player->cInput->m_Left && left - m_Player->cTransform->m_Speed > 0.0f)
	{
		m_Player->cTransform->m_Pos.x -= m_Player->cTransform->m_Speed;
	}

	if (m_Player->cInput->m_Up && top - m_Player->cTransform->m_Speed > 0.0f)
	{
		m_Player->cTransform->m_Pos.y -= m_Player->cTransform->m_Speed;
	}

	if (m_Player->cInput->m_Down && botton + m_Player->cTransform->m_Speed < m_Window.getSize().y)
	{
		m_Player->cTransform->m_Pos.y += m_Player->cTransform->m_Speed;
	}
}

void Game::UpdateEntitiesMove()
{
	for (SPEntity& e : m_EntityManager.GetAllEntities())
	{
		if (e->GetTag() == "player")
			continue;

		UpdateEnemyMove(e);
	}
}

void Game::UpdateEnemyMove(SPEntity& e)
{
	if (!m_MovementEnabled)
		return;

	if (!e->cShape)
		return;

	if (e->cTransform->m_Velocity.x == 0.0f && e->cTransform->m_Velocity.y == 0.0f)
		return;

	sf::CircleShape& circle = e->cShape->m_Circle;

	float left = e->cTransform->m_Pos.x - circle.getRadius();
	float right = e->cTransform->m_Pos.x + circle.getRadius();
	float top = e->cTransform->m_Pos.y - circle.getRadius();
	float botton = e->cTransform->m_Pos.y + circle.getRadius();

	if (left <= 0 || right >= m_Window.getSize().x)
	{
		e->cTransform->m_Velocity.x *= -1.0f;
	}

	if (top <= 0 || botton >= m_Window.getSize().y)
	{
		e->cTransform->m_Velocity.y *= -1.0f;
	}

	e->cTransform->m_Pos += e->cTransform->m_Velocity;
}