#pragma once
#include <memory>

#include "entity.h"
#include "gameengine.h"

class Physics
{
public:
	static Vec2 GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b, GameEngine& game);
	Vec2 GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};

