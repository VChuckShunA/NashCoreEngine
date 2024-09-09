#include "EntityManager.h"
#include "Entity.h"
#include <algorithm>

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
	for (const auto e : m_entitiesToAdd) {
		// Push to all entities
		m_entities.push_back(e);

		// Push to map of vector of entities based on tag
		m_entityMap[e->tag()].push_back(e);
	}

	// clear queue when all are added
	m_entitiesToAdd.clear();

	// remove dead entites from the vector of all entities
	removeDeadEntities(m_entities);

	// remove dead entities from each vector in the entity map
	//C++ 20 way of iterating through [key,value] pairs in a map
	for (auto& [tag, entityVec_] : m_entityMap) {
		removeDeadEntities(entityVec_);
	}
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	//remove all dead entities from the input vector
	vec.erase(std::remove_if(vec.begin(), vec.end(),
		[](auto it) { return !it->isActive(); }),
		vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	// Don't add to entities, make a queue instead. Entities are added in update
	m_entitiesToAdd.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities()
{
	return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
	//return the correct vector from the map, indexed by tag
	return m_entityMap[tag];
}

const std::map<std::string, EntityVec>& EntityManager::getEntityMap()
{
	// TODO: insert return statement here
	//itereate through entities and retrieve by tag
	return m_entityMap;
}
