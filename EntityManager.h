#pragma once

#include "Entity.h"
#include <map>
#include <memory>
#include <vector>

typedef std::vector<std::shared_ptr<Entity> > EntityVec;
typedef std::map<std::string, EntityVec> entityMap;

class EntityManager
{
	EntityVec m_entities;	    // vector of all entities
	EntityVec m_entitiesToAdd;  // queue of entities to be added
	entityMap m_entityMap;
	size_t m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);

public:
	EntityManager();

	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	const EntityVec& getEntities();
	const EntityVec& getEntities(const std::string& tag);
	const std::map<std::string, EntityVec>& getEntityMap();
};