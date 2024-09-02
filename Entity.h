#pragma once
#include <memory>
#include "Component.h"
#include "CShape.h"

class Entity
{
private:
	const size_t m_Id = 0;
	const std::string m_Tag{ "Default" };
	bool m_Alive = true;

	std::vector<Component> m_Components;	
	Entity() {};

public:
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CName>		cName;
	std::shared_ptr<CShape>		cShape;
	std::shared_ptr<CBBox>		cBBox;

	Entity(const std::string& tag, size_t id);

	const std::string& GetTag() { return m_Tag; };

	void MarkAsDead() { m_Alive = false; };
	bool IsAlive() const { return m_Alive; };
	//void AddComponent<T>(args);
	//void GetComponent<T>(args);

};

