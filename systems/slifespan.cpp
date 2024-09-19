#include "../game.h"

void Game::sLifeSpan()
{
	if (m_ImmortalEnemies)
		return;

	for (SPEntity& e : m_EntityManager.GetAllEntities())
	{
		UpdateLife(e);
	}
}

void Game::UpdateLife(SPEntity& e)
{
	if (!e->IsActive())
		return;

	if (!e->cLifeSpan)
		return;

	e->cLifeSpan->m_Remaining--;

	if (e->cLifeSpan->m_Remaining == 0)
	{
		e->Destroy();
	}

	float opacity = 255 * ((float)e->cLifeSpan->m_Remaining / (float)e->cLifeSpan->m_Total);

	sf::Color fillcolor = e->cShape->m_Circle.getFillColor();
	fillcolor.a = opacity;
	e->cShape->m_Circle.setFillColor(fillcolor);

	sf::Color outlinecolor = e->cShape->m_Circle.getFillColor();
	outlinecolor.a = opacity;
	e->cShape->m_Circle.setOutlineColor(outlinecolor);
}