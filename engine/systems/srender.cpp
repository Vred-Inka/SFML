#include "../game.h"

void Game::sRender()
{
	m_Window.clear();

	DrawEntity(m_Player);

	for (SPEntity& e : m_EntityManager.GetAllEntities())
	{
		DrawEntity(e);
	}

	m_Window.draw(m_Text);

	//draw the ui last
	ImGui::SFML::Render(m_Window);

	m_Window.display();
}

void Game::DrawEntity(SPEntity& e)
{
	e->cShape->m_Circle.setPosition(e->cTransform->m_Pos.x, e->cTransform->m_Pos.y);
	e->cShape->m_Circle.setRotation(e->cShape->m_Circle.getRotation()+0.1f);

	if (e->cSuperPower && e->cSuperPower->m_RemainingFadeTime > 0)
	{
		e->cSuperPower->m_Circle.setPosition(e->cSuperPower->m_Pos.x - e->cSuperPower->m_Radius,
			e->cSuperPower->m_Pos.y - e->cSuperPower->m_Radius);
		m_Window.draw(e->cSuperPower->m_Circle);

		float opacity = 255 * ((float)e->cSuperPower->m_RemainingFadeTime
			/ (float)e->cSuperPower->m_FadeTime);

		sf::Color outlinecolor =sf::Color::Red;
		outlinecolor.a = opacity;
		e->cSuperPower->m_Circle.setOutlineColor(outlinecolor);
	}

	m_Window.draw(e->cShape->m_Circle);
}