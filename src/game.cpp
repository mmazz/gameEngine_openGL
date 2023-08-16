#include "game.h"
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>

// The Width of the screen
float SCREEN_WIDTH = 1920.0f;
// The height of the screen
float SCREEN_HEIGHT = 1080.0f;
float ratio =  SCREEN_WIDTH/SCREEN_HEIGHT;

const size_t MaxQuadsCount = 1000;

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

static Vertex* CreateQuad(Vertex* target, glm::vec3 pos, float size)
{
    float x = pos[0];
    float y = pos[1];
    target->Position = {x-size, y-size};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x+size, y-size};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x+size, y+size};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x-size, y+size};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    return target;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = (float)width;
    SCREEN_HEIGHT = (float)height;
    ratio =  (float)width/(float)height;
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
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
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
    const size_t MaxVertexCount = MaxQuadsCount * 4;
    const size_t MaxIndexCount = MaxQuadsCount * 6;

    glGenVertexArrays(1, &m_QuadVA);
    glGenBuffers(1, &m_QuadVB);
    glGenBuffers(1, &m_QuadIB);
    glBindVertexArray(m_QuadVA);

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr , GL_DYNAMIC_DRAW);


    // el primero es el componente que estamos seteando, en este caso el primero osea 0
    // despues es la cantidad de elementos que tiene, tenemos 3 posiciones
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glEnableVertexAttribArray(1);

    uint32_t indices[MaxIndexCount];
    uint32_t offset = 0;
    for (size_t i=0; i<MaxIndexCount; i+=6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;

        offset += 4;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // position attribute                                                              //
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    spawnPlayer();
 //   spawnEnemy();
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
    glDeleteVertexArrays(1, &m_QuadVA);
    glDeleteBuffers(1, &m_QuadVB);
    glDeleteBuffers(1, &m_QuadIB);
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
    entity->cShape = std::make_shared<CShape>();
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
    float ex = rand() % 1-0.5f;
    float ey = rand() % 1-0.5f;
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(ex, ey,0.0f),
                                                     glm::vec3(0.01f, 0.05f,0.0f), 0.0f);
//    //falta meter las variables del config
    m_lastEnemySpawnTime = m_currentFrame;

    entity->cShape = std::make_shared<CShape>(0.1f);
}
// Tieene que hacer draw de todo...
void Game::sRender()
{
    if (rotate < 6.30f)
        rotate += 0.25f;
    else
        rotate = 0.0f;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    uint32_t indexCount = 0;
    std::array<Vertex, MaxQuadsCount * 4> vertices;
    Vertex* buffer = vertices.data();

    glm::mat4 model(1.0f);
    m_ourShader->use();
    model =  scale(model, glm::vec3( 1.0f/ratio, 1.0f, 1.0f));
    glm::mat4 rotation = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f)); // where x, y, z is axis of rotation (e.g. 0 1 0)
    for (auto e: m_entities.getEntities())
    {
        if(e->cTransform)
        {
            buffer = CreateQuad(buffer, e->cTransform->m_pos, e->cShape->m_size);

            indexCount += 6;
        }

    }
    glm::mat4 u_MPV =  model;
    // Esto aca seria para algo global como la camara...
    m_ourShader->setMat4("u_MPV",  u_MPV);

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glClear(GL_COLOR_BUFFER_BIT);



    glBindVertexArray(m_QuadVA);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
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
        if(m_player->cTransform->m_pos[1]<1.0f-m_player->cShape->m_size)
            m_player->cTransform->m_pos[1] += m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_S])
    {
        if(m_player->cTransform->m_pos[1]>-1.0f+m_player->cShape->m_size)
            m_player->cTransform->m_pos[1] -= m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_A])
    {
        if(m_player->cTransform->m_pos[0]>-1.0f-m_player->cShape->m_size*ratio)
            m_player->cTransform->m_pos[0] -= m_player->cTransform->m_velocity[0];
    }
    if(m_player->cInput->Keys[GLFW_KEY_D])
    {
        if(m_player->cTransform->m_pos[0]<1.0f+m_player->cShape->m_size*ratio)
            m_player->cTransform->m_pos[0] +=m_player->cTransform->m_velocity[0];
    }
    for (auto e: m_entities.getEntities(Enemy))
    {
        if(e->cTransform->m_pos[1]>1.0f-e->cShape->m_size)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[1]<-1.0f+e->cShape->m_size)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[0]<-1.0f-e->cShape->m_size*ratio)
            e->cTransform->m_velocity[0] = -1*e->cTransform->m_velocity[0];

        if(e->cTransform->m_pos[0]>1.0f+e->cShape->m_size*ratio)
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
    if ((m_currentFrame-m_lastEnemySpawnTime)%100==0)
    {
        spawnEnemy();
    }
}


// RGBA el A es de alpha de transparencia.
// Requere que tenga CTransform y CShape
//
//
void Game::sUserInput()
{

}


