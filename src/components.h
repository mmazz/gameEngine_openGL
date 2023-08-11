#pragma once
// La C es de component, y depsues el nombre
#include <glm/glm.hpp>

enum polygons
{
    Triangle,
    Square,
    Pentagon,
    Hexagon
};



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


    float m_vertices[1024];
    unsigned int m_indices[100];
public:
    // Tengo que armarme vertex arrays?
    CShape(const enum polygons type_poly, glm::vec2 pos, glm::vec2 size,  glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
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
    bool Keys[1024];
    bool escape = false;
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
