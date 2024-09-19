#include "Entity.h"

Entity::Entity(const std::string& tag, size_t id) :
	m_Tag(tag), m_Id(id)
{
}

bool Entity::HasCollision(Entity& e)
{
	if (!cCollision || !e.cCollision)
		return false;

	if (!cTransform || !e.cTransform)
		return false;

	float r = cCollision->m_Radius + e.cCollision->m_Radius;
	Vec2 d{ e.cTransform->m_Pos.x - cTransform->m_Pos.x,
		e.cTransform->m_Pos.y - cTransform->m_Pos.y };

	return d.x*d.x + d.y*d.y < r*r;
}

bool Entity::HasCollision(const Vec2& pos, int radius)
{
	if (!cCollision)
		return false;

	if (!cTransform)
		return false;

	float r = cCollision->m_Radius + radius;
	Vec2 d{ cTransform->m_Pos.x - pos.x,
		cTransform->m_Pos.y - pos.y};

	return d.x * d.x + d.y * d.y < r * r;
}