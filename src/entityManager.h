#pragma once

#include <vector>
#include <algorithm>
#include <map>
#include "entity.h"


typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<enum tag, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;
    // Esto es para no tener iterator invalidation, basicamente
    // guardo la info y hago los cambiso al principio del siguiente frame
    EntityVec m_entitiesToAdd;
    EntityVec m_entitiesToDelete;
    EntityMap m_entityMap;
    // para nunca repetir el id.
    size_t    m_totalEntities = 0;
    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const enum tag& tag);

    void addDeadEntity(std::shared_ptr<Entity> e);
    const EntityVec& getEntities();
    const EntityVec& getEntities(const enum tag& tag);
};
