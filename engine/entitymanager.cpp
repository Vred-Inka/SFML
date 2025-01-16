#include "entitymanager.h"


EntityManager::EntityManager()
{
}

void EntityManager::Init()
{
}

void EntityManager::Update()
{
	for (std::shared_ptr<Entity>& e : m_EntitiesToAdd)
	{
		m_Entities.push_back(e);
		m_EntityMap[e->GetTag()].push_back(e);
	}

	m_EntitiesToAdd.clear();

	RemoveDeadEntities(m_Entities);	

	for (auto& p : m_EntityMap)
	{
		RemoveDeadEntities(p.second);
	}
}

//TODO
void EntityManager::RemoveDeadEntities(EntityVec& vec)
{	
	if (vec.empty())
		return;

	for (int i = (int)vec.size() - 1; i >= 0; i--)
	{
		if (!vec[i]->IsActive())
		{
			vec.erase(vec.begin() + i);
		}
	}
}

SPEntity EntityManager::AddEntity(const std::string& tag)
{
	SPEntity e = std::make_shared<Entity>(tag, m_TotalEntityes++);
	m_EntitiesToAdd.push_back(e);
	return e;
}

const EntityVec& EntityManager::GetEntities()
{
	return m_Entities;
}

EntityVec& EntityManager::GetAllEntities()
{
	return m_Entities;
}

EntityVec& EntityManager::GetEntities(const std::string& tag)
{
	return m_EntityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::GetEntityMap()
{
	return m_EntityMap;
}

void EntityManager::KillEntity(const std::string& tag)
{
	for (std::shared_ptr<Entity>& e : m_Entities)
	{
		if (tag == e->GetTag())
		{
			e->Destroy();
		}
	}
}