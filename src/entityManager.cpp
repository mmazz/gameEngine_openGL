#include "entityManager.h"
#include <filesystem>
#include <iterator>
// Tener cuidado con Iterator Invalitaion
// Basicamente modificar el contenido de una collecion mientras lo itero
// Tiene muchas veces comportamiento no definido.
// Ej: hago un push_back del vector, y requiere hacer un resize, eso inavilita todos los
// punteros e iteradores...
// Para eso hacemos las cosas que posiblen generen esto es retrasar ese comportamiento
// Basicamente me guardo lo que quiero hacer y despues de terminar todo el loop
// hago todos los cambios

EntityManager::EntityManager()
{

}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    std::vector<int> entitys_dead;
    int count = 0;
    for (auto e: vec)
    {
        if (!e->m_active)
            entitys_dead.push_back(count);
        count++;
    }
    for ( size_t i=entitys_dead.size()-1; i>=0 ; i++)
    {
        vec.erase(vec.begin()+i);
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const enum tag& tag)
{
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_entitiesToAdd.push_back(e);
    return e;
}

void EntityManager::update()
{
    for (auto e: m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }

    m_entitiesToAdd.clear();

//    removeDeadEntities(m_entities);
    for (auto& [tag, entityVec] : m_entityMap)
    {
  //      removeDeadEntities(entityVec);
    }
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const enum tag& tag)
{
    return m_entityMap[tag];
}


