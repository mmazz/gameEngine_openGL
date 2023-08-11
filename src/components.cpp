#include "components.h"


float vertices_triangle[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
unsigned int indices_triangle[] = {
    0, 1, 2,
};


float vertices_square[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left
};

unsigned int indices_square[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

float vertices_pentagon[] = {
    -0.7f,  -1.0f, 0.0f,
    -1.0f, 0.3f, 0.0f,
    0.0f,  1.0f, 0.0f,
    1.0f,  0.3f, 0.0f,
    0.7f,  -1.0f, 0.0f
};

float vertices_hexagon[] = {
    0.0f,   0.0f, 0.0f,    //center
    -0.5f,   1.0f, 0.0f,    // left top
    0.5f,   1.0f, 0.0f,    // right top
    1.0f,   0.0f, 0.0f,    // right
    0.5f,   -1.0f, 0.0f,    // right bottom (notice sign)
    -0.5f,  -1.0f, 0.0f,    // left bottom
    -1.0f,   0.0f, 0.0f,     // left
};

CShape::CShape(const enum polygons type_poly, glm::vec2 pos, glm::vec2 size,  glm::vec3 color, glm::vec2 velocity)
{
        if (type_poly == Triangle)
        {
            for (int i = 0; i< 9; i++)
                m_vertices[i] = vertices_triangle[i];
            for (int i = 0; i< 3; i++)
                m_indices[i] = indices_triangle[i];
        }
        if (type_poly == Square)
        {
            for (int i = 0; i< 12; i++)
                m_vertices[i] = vertices_square[i];
            for (int i = 0; i< 6; i++)
                m_indices[i] = indices_square[i];
        }
    }
