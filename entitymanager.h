#pragma once
#include <memory>
#include <vector>
#include "entity.h"

typedef std::shared_ptr<Entity> SPEntity;
typedef std::vector<SPEntity> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
	EntityVec m_Entities;
	EntityVec m_EntitiesToAdd;
	EntityMap m_EntityMap;
	rsize_t m_TotalEntityes = 0;

	void RemoveDeadEntities(EntityVec& vec);

public:
	EntityManager();

	void Init();
	void Update();

	void KillEntity(const std::string& tag);
	void SpawnEnemy();

	SPEntity AddEntity(const std::string& tag);
	const EntityVec& GetEntities();
	const EntityVec& GetEntities(const std::string& tag);  
	const std::map<std::string, EntityVec>& GetEntityMap();

};

