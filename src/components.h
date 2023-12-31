#pragma once
#include <glm/glm.hpp>


// Not used for now
enum polygons
{
    Triangle,
    Square,
    Pentagon,
    Hexagon
};

class CTransform
{
public:
    glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_pos_init = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_pos_old = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_velocity = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_velocity_init = {0.0f, 0.0f, 0.0f};

    float angle = 0;

    CTransform(const glm::vec3& p, const glm::vec3& v, float a)
        :m_pos(p), m_pos_init(p),m_pos_old(p-v), m_velocity(v),  m_velocity_init(v), angle(a) {}
};


class CShape
{
public:
    glm::vec3 m_size;
    CShape():m_size(0.25f, 0.25f, 0.0f){}
    CShape(float sizeX, float sizeY):m_size(sizeX, sizeY, 0.0f) {}
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
    CScore(int s): score(s) {}
};

class CLifeSpan
{
public:
    int remaining = 0;
    int total     = 0;
    CLifeSpan(int total) : remaining(total), total(total) {}
};
