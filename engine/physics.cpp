#include "physics.h"
#include "components.h"

#include <math.h>

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, GameEngine& game)
{
	float ox = 0.0f;
	float oy = 0.0f;

	if (a->HasComponent<CBoundingBox>() && b->HasComponent<CBoundingBox>())
	{
		Vec2 aPos = a->GetComponent<CTransform>().m_Pos;
		Vec2 bPos = b->GetComponent<CTransform>().m_Pos;

		Vec2 delta = Vec2(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
		ox = a->GetComponent<CBoundingBox>().m_HalfSize.x + b->GetComponent<CBoundingBox>().m_HalfSize.x - delta.x;
		oy = a->GetComponent<CBoundingBox>().m_HalfSize.y + b->GetComponent<CBoundingBox>().m_HalfSize.y - delta.y;

	/*	sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(aPos.x + a->GetComponent<CBoundingBox>().m_HalfSize.x , aPos.y)),
			sf::Vertex(sf::Vector2f(aPos.x, aPos.y)),
			sf::Vertex(sf::Vector2f(bPos.x, bPos.y)),
			sf::Vertex(sf::Vector2f(bPos.x, bPos.y + b->GetComponent<CBoundingBox>().m_HalfSize.x))
		};

		game.GetWindow().draw(line, 4, sf::Lines);*/

		if (ox > 0 && oy > 0)
		{
			CBoundingBox& abbox = a->GetComponent<CBoundingBox>();
			CTransform& atransform = a->GetComponent<CTransform>();
			sf::RectangleShape arect;
			arect.setSize(sf::Vector2f(abbox.m_Size.x, abbox.m_Size.y));
			arect.setOrigin(sf::Vector2f(abbox.m_HalfSize.x, abbox.m_HalfSize.y));
			arect.setPosition(atransform.m_Pos.x, atransform.m_Pos.y);
			arect.setFillColor(sf::Color(0, 0, 0, 0));
			arect.setOutlineThickness(3);
			arect.setOutlineColor(sf::Color::Red);
			game.GetWindow().draw(arect);


			CBoundingBox& bbox = b->GetComponent<CBoundingBox>();
			CTransform& transform = b->GetComponent<CTransform>();
			sf::RectangleShape rect;
			rect.setSize(sf::Vector2f(bbox.m_Size.x, bbox.m_Size.y));
			rect.setOrigin(sf::Vector2f(bbox.m_HalfSize.x, bbox.m_HalfSize.y));
			rect.setPosition(transform.m_Pos.x, transform.m_Pos.y);
			rect.setFillColor(sf::Color(0, 0, 0, 0));
			rect.setOutlineThickness(3);
			rect.setOutlineColor( sf::Color::Blue);
			game.GetWindow().draw(rect);


			std::string s = std::to_string((int)ox) + " " + std::to_string((int)oy);
			sf::Text text(s, game.GetAssets().GetFont("Mario"));
			text.setCharacterSize(16);
			text.setFillColor(sf::Color::Red);
			text.setPosition(bPos.x - bbox.m_HalfSize.x, bPos.y - bbox.m_HalfSize.y - 20);
			game.GetWindow().draw(text);

		}
	}

	return Vec2(ox, oy);
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
	float ox = 0.0f;
	float oy = 0.0f;

	if (a->HasComponent<CBoundingBox>() && b->HasComponent<CBoundingBox>())
	{
		Vec2 aPos = a->GetComponent<CTransform>().m_Pos;
		Vec2 bPos = b->GetComponent<CTransform>().m_Pos;

		Vec2 delta = Vec2(abs(aPos.x - bPos.x), abs(aPos.y - bPos.y));
		ox = a->GetComponent<CBoundingBox>().m_HalfSize.x + b->GetComponent<CBoundingBox>().m_HalfSize.x - delta.x;
		oy = a->GetComponent<CBoundingBox>().m_HalfSize.y + b->GetComponent<CBoundingBox>().m_HalfSize.y - delta.y;
	}
	//TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
	// previous overlap uses the entity's previous position

	return Vec2(ox, oy);
}