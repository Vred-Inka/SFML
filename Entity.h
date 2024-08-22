#pragma once
#include <memory>
#include "Component.h"
#include "CShape.h"

class Entity
{
	std::vector<Component> mComponents;

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName>		cName;
	std::shared_ptr<CShape>		cShape;
	std::shared_ptr<CBBox>		cBBox;

	Entity() {};
	//void AddComponent<T>(args);
	//void GetComponent<T>(args);

};

