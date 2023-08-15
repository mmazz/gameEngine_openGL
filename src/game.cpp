#include "game.h"
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1920;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 1080;

struct Vec2
{
    float x, y;
};

struct Vec4
{
    float x, y, z, w;
};
struct Vertex
{
    Vec2 Position;
    Vec4 Color;
};

float rotate = 0.0f;
float size = 0.25f;
static std::array<Vertex, 4> CreateQuad(glm::vec3 pos)
{
    float x = pos[0];
    float y = pos[1];
    Vertex v0;
    v0.Position = {x-size, y-size};
    v0.Color = {0.18f+x, 0.1f, 0.3f, 1.0f};
    Vertex v1;
    v1.Position = {x+size, y-size};
    v1.Color = {0.18f+x, 0.1f, 0.3f, 1.0f};
    Vertex v2;
    v2.Position = {x+size, y+size};
    v2.Color = {0.18f+x, 0.1f, 0.3f, 1.0f};
    Vertex v3;
    v3.Position = {x-size, y+size};
    v3.Color = {0.18f+x, 0.1f, 0.3f, 1.0f};
    return { v0, v1, v2, v3 };
}
// todo hacer que los vertex entren por el config file
Game::Game(const std::string& config, const char* vertexPath, const char* fragmentPath)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);

    m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MatiDo", nullptr, nullptr);
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // deberia terminar...
    }
    // Estas dos variables deberian venir del config filae
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    m_ourShader = new Shader(vertexPath, fragmentPath);

    init(config);
}

Game::~Game()
{
    delete m_ourShader;
}
void Game::init(const std::string& path)
{
    const size_t MaxQuadsCount = 10;
    const size_t MaxVertexCount = MaxQuadsCount * 4;
    const size_t MaxIndexCount = MaxQuadsCount * 6;
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr , GL_DYNAMIC_DRAW);


    // el primero es el componente que estamos seteando, en este caso el primero osea 0
    // despues es la cantidad de elementos que tiene, tenemos 3 posiciones
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (int i=0; i<MaxIndexCount; i+=6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 3 + offset;
        indices[i + 3] = 1 + offset;
        indices[i + 4] = 2 + offset;
        indices[i + 5] = 3 + offset;

        offset += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // position attribute                                                              //
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    spawnPlayer();
    spawnEnemy();
    //read variables...
    // create window
    // set framerate limit
    //
}

void Game::run()
{
    // agregar que cuando pausamos algunos system no tienen que andar
    // y otros si como el render.
    while (!glfwWindowShouldClose(m_window))
    {
        float currentFrame = glfwGetTime();
        m_entities.update();

        if(!m_paused)
        {
            sEnemySpawner();
            sMovement();
            sCollision();
            sUserInput();
        }

        sRender();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
        m_currentFrame++;
    }
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

void Game::setPaused(bool paused)
{
    m_paused = paused;
}

void Game::spawnPlayer()
{
    // falta agregar demas propiedades del config
    auto entity = m_entities.addEntity(Player);
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(0.0f, 0.0f,0.0f),
                                                      glm::vec3(0.01f, 0.01f,0.0f), 0.0f);
//   entity->cShape = std::make_shared<CShape>();
//    // falta meter inputs
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
    for (int i=0; i<1024; i++)
    {
        m_player->cInput->Keys[i] = false;
    }


}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity(Enemy);
//    float ex = rand() % window.Width;
 //   float ey = rand() % window.Height;
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(0.0f, 0.0f,0.0f),
                                                     glm::vec3(0.01f, 0.05f,0.0f), 0.0f);
//    //falta meter las variables del config
//    m_lastEnemySpawnTime = m_currentFrame;

}
// Tieene que hacer draw de todo...
void Game::sRender()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // render the triangle
    m_ourShader->use();
    glBindVertexArray(m_VAO);

    Vertex vertices[8];

    glm::mat4 model(1.0f);
    size_t temp = 0;
    auto q = CreateQuad(glm::vec3(1.0f));
    if (rotate < 6.30f)
        rotate += 0.25f;
    else
        rotate = 0.0f;
    for (auto e: m_entities.getEntities())
    {
        if(e->cTransform)
        {
            q = CreateQuad(e->cTransform->m_pos);
            memcpy(vertices + temp, q.data(), q.size() * sizeof(Vertex));
            temp += q.size();
        }
        m_ourShader->setMat4("u_MPV", model);

      //  glDrawArrays(GL_TRIANGLES, 0, 6);
        // 6=cantidad de indices
    }
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, nullptr);
}

//    for (auto e : m_entities.getEntities())
//    {
//        // si no tiene, es que tiene un nullptr y va a ser falso y no entra
//        if (e->cTransform && e->cShape)
//            break;
//          //  std::cout << " " << std::endl;
//    //          e.cShape->shape.setPosition(e.cTransform->pos);
//    //          window.draw(e.cShape->shape);
//
//    }




void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{

}
void Game::spawnBullet(std::shared_ptr<Entity> e, const glm::vec2& target)
{

}

// requiere que tenga CTransform
void Game::sMovement()
{
    if(m_player->cInput->Keys[GLFW_KEY_W])
    {
        if(m_player->cTransform->m_pos[1]<1.0f)
            m_player->cTransform->m_pos[1] += m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_S])
    {
        if(m_player->cTransform->m_pos[1]>-1.0f)
            m_player->cTransform->m_pos[1] -= m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_A])
    {
        if(m_player->cTransform->m_pos[0]>-1.0f)
            m_player->cTransform->m_pos[0] -= m_player->cTransform->m_velocity[0];
    }
    if(m_player->cInput->Keys[GLFW_KEY_D])
    {
        if(m_player->cTransform->m_pos[0]<1.0f)
            m_player->cTransform->m_pos[0] +=m_player->cTransform->m_velocity[0];
    }
    for (auto e: m_entities.getEntities(Enemy))
    {
        if(e->cTransform->m_pos[1]>1.0f)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[1]<-1.0f)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[0]<-1.0f)
            e->cTransform->m_velocity[0] = -1*e->cTransform->m_velocity[0];

        if(e->cTransform->m_pos[0]>1.0f)
            e->cTransform->m_velocity[0] = -1*e->cTransform->m_velocity[0];

        e->cTransform->m_pos += e->cTransform->m_velocity;
       // std::cout << e->cTransform->m_pos[0] << std::endl;
    //    std::cout << e->cTransform->m_pos[1] << std::endl;
    }

}

void Game::sCollision()
{
}

void Game::sEnemySpawner()
{
    // agregar par que cada x frames se meta uno.
 //   if (m_currentFrame - m_lastEnemySpawnTime)
 //       spawnEnemy();
}


// RGBA el A es de alpha de transparencia.
// Requere que tenga CTransform y CShape
//
//
void Game::sUserInput()
{

}


