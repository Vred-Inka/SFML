#include "entitymanager.h"


EntityManager::EntityManager()
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

	//remove dead entities from the vector of all entities
	RemoveDeadEntities(m_Entities);

	//reove dead entities from eacch vector in the entitie map
	//c++20 way inerating through [key, value] pairs in a map
	/*for (auto& [tag, entityVec] : m_EntityMap)
	{
		RemoveDeadEntities(entityVec);
	}
	*/

	//clear entity that is not alive
	for (std::shared_ptr<Entity>& e : m_Entities)
	{
		//TODO removing with remove if

		if (!e->IsActive())
		{
			//m_Entities.erase(e)
			//m_Entities.erase(e);
			//m_EntityMap[e->GetTag()].push_back(e);
		}
	}
}

//TODO
void EntityManager::RemoveDeadEntities(EntityVec& vec)
{
	//TODO: remove all dead entities from the input vec
	// this is called by update() function
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


const EntityVec& EntityManager::GetEntities(const std::string& tag)
{
	//TODO: return correct vec
	return m_Entities;
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


void EntityManager::SpawnEnemy()
{
	SPEntity e = AddEntity("enemy");
	//e->cTransform = std::make_shared<CTransform>(args);
	//e->cShape = std::make_shared<CShape>(args);
}