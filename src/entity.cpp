#include "entity.h"

Entity::Entity(const size_t id, const enum tag& tag)
    : m_id(id), m_tag(tag) {}

bool Entity::isActive() const
{
    return m_active;
}

const enum tag& Entity::tag() const
{
    return m_tag;
}

const size_t Entity::id() const
{
    return m_id;
}

void Entity::destroy()
{
    m_active = false;
}
