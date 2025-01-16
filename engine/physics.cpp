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