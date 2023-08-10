#pragma once
// La C es de component, y depsues el nombre
#include <glm/glm.hpp>

// Basicamente como vamos a movernos en el espacio
class CTransform
{
public:
    glm::vec2 m_pos = {0.0f, 0.0f};
    glm::vec2 m_velocity = {0.0f, 0.0f};
    float angle = 0;

    CTransform(const glm::vec2& p, const glm::vec2& v, float a)
        :m_pos(p), m_velocity(v), angle(a) {}
};


class CShape
{
public:
    // Tengo que armarme vertex arrays?
    CShape() {}
};


class CCollision
{
    public:
    float radius = 0;
    CCollision(float r): radius(r) {}
};

class CInput
{
public:
    bool up    = false;
    bool left  = false;
    bool right = false;
    bool down  = false;
    bool shoot = false;
    CInput() {}
};

class CScore
{
public:
    int score = 0;
    // tengo que armarme vertex arrays?
    CScore(int s): score(s) {}
};

class CLifeSpan
{
public:
    int remaining = 0;
    int total     = 0;
    // tengo que armarme vertex arrays?
    CLifeSpan(int total) : remaining(total), total(total) {}
};
