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
	EntityMap m_EntityMap;

	EntityVec m_ToAdd;

	rsize_t m_TotalEntityes = 0;

public:
	EntityManager() {};

	void Update();


	SPEntity AddEntity(const std::string& tag);
	void KillEntity(const std::string& tag);

	void SpawnEnemy();

	EntityVec& GetEntities();
	EntityVec& GetEntities(const std::string& tag);  
};

