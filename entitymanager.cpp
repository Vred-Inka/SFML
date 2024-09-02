#include "entitymanager.h"

SPEntity EntityManager::AddEntity(const std::string& tag)
{
	SPEntity e = std::make_shared<Entity>(tag, m_TotalEntityes++);
	m_ToAdd.push_back(e);
	return e;
}

void EntityManager::KillEntity(const std::string& tag)
{
	for (std::shared_ptr<Entity>& e : m_Entities)
	{
		if (tag == e->GetTag())
		{
			e->MarkAsDead();
		}
	}
}

void EntityManager::Update()
{
	for (std::shared_ptr<Entity>& e : m_ToAdd)
	{
		m_Entities.push_back(e);
		m_EntityMap[e->GetTag()].push_back(e);
	}

	m_ToAdd.clear();

	//clear entity that is not alive
	for (std::shared_ptr<Entity>& e : m_Entities)
	{
		//TODO removing with remove if

		if (!e->IsAlive())
		{
			//m_Entities.erase(e)
			//m_Entities.erase(e);
			//m_EntityMap[e->GetTag()].push_back(e);
		}
	}
}

void EntityManager::SpawnEnemy()
{
	SPEntity e = AddEntity("enemy");
	//e->cTransform = std::make_shared<CTransform>(args);
	//e->cShape = std::make_shared<CShape>(args);
}