#pragma once
#include "entitymanager.h"
class Physics
{
public:
	static bool IsCollision(const Entity& e1, const Entity& e2)
	{
		return false;
	}
};

static void UpdateCollisions()
{
	EntityVec bullets;
	EntityVec tiles;

	for (std::shared_ptr<Entity>& b : bullets)
	{
		for (std::shared_ptr<Entity>& t : tiles)
		{
			if (Physics::IsCollision(*b, *t))
			{
				//bullets.erase(b);
			}
		}
	}

	/*for (auto b : m_Entities.GetEntities("bullet"))
	{
		for (auto e : m_Entities.GetEntities("enemy"))
		{
			if (Physics::IsCollision(*b, *e))
			{
				b->MarkAsDead();
				e->MarkAsDead();
			}
		}
	}
	*/
}
