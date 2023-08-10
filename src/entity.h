#pragma once
#include <memory>
#include "components.h"

enum tag {
    Default,
    Tile,
    Player,
    Bullet,
    Enemy
};

class Entity
{
    friend class EntityManager;

    bool           m_active = true;
    size_t         m_id    = 0;
    enum tag       m_tag   = Default;
    // Al poner el constructor como privado nadie puede generar una entity
    // en el codigo. Como solo queremos que EntityManager pueda, lo hacemos
    // clase amiga. El problema es que ya no puedo hacer cosas como
    // std::make_shared<Entity>(args). Usamops auto e= std::shared_ptr<Entity>(new Entity(args))
    Entity(const enum tag& tag, const size_t id);

public:
    std::shared_ptr<CTransform> cTransform;
    std::shared_ptr<CShape>     cShape;
    std::shared_ptr<CCollision> cCollision;
    std::shared_ptr<CInput>     cInput;
    std::shared_ptr<CScore>     cScore;
    std::shared_ptr<CLifeSpan>  cLifeSpan;
    bool isActive() const;
    const enum tag& tag() const;
    const size_t id() const;
    void destroy();
    // No tengo destructor por que los shared_ptr se resuelven solos
};

