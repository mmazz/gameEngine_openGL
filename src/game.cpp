#include "game.h"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <glm/ext/matrix_clip_space.hpp>

float SCREEN_WIDTH = 1920.0f;
float SCREEN_HEIGHT = 1080.0f;
float ratio =  SCREEN_WIDTH/SCREEN_HEIGHT;

const bool SIMULATION = false;

const size_t MaxQuadsCount = 10000;

const float ENEMY_SIZE = 0.20f;
const float BULLET_SIZE = 0.01f;

const size_t FPS_SPAWN = 10;
const float FPS_CAP = 60.0f;
const float ABSORTION = 0.2f;

const double maxFPS = 60.0;
const double maxPeriod = 1.0 / maxFPS;

const glm::vec3 gravity_vec = glm::vec3(0.0f, -40.0f, 0.0f);
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCREEN_WIDTH = (float)width;
    SCREEN_HEIGHT = (float)height;
    ratio =  (float)width/(float)height;
}

struct Vec2
{
    float x, y;
};

struct Vec3
{
    float x, y, z;
};

struct Vec4
{
    float x, y, z, w;
};

struct Vertex
{
    Vec3 Position;
    Vec4 Color;
};


float rotate = 0.0f;
float RandomNumber(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

static Vertex* CreateQuad(Vertex* target, glm::vec3 pos, Vec2 size)
{
    float x = pos[0];
    float y = pos[1];
    target->Position = {x, y, 1.0f};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x+size.x, y, 1.0f};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x+size.x, y+size.y, 1.0f};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    target->Position = {x, y+size.y, 1.0f};
    target->Color = {0.18f+x, 0.2f, 0.3f+y, 1.0f};
    target++;

    return target;
}

static void QuadRotation(Vertex* target, glm::vec3 pos, Vec2 size, float angle)
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, pos);
    model = glm::translate(model, glm::vec3(0.5f*size.x, 0.5f*size.y, 0.0f));
    model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, -pos);
    model = glm::translate(model, glm::vec3(-0.5f*size.x, -0.5f*size.y, 0.0f));

    for (int i=0; i<4; i++)
    {
        target--;
        Vec3* position = &target->Position;
        glm::vec4 res = model*glm::vec4(glm::vec3(position->x, position->y, position->z), 1.0f);
        position->x = res[0];
        position->y = res[1];
        position->z = res[2];
    }

}



Game::Game(const std::string& config, const char* vertexPath, const char* fragmentPath)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_RESIZABLE, false);

    this->m_window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "MatiDo", nullptr, nullptr);
    glfwMakeContextCurrent(this->m_window);
    glfwSetFramebufferSizeCallback(this->m_window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        // return?
    }
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    m_ourShader = new Shader(vertexPath, fragmentPath);

    init(config);
}

Game::~Game()
{
    delete m_ourShader;
}

GLFWwindow* Game::getWindow()
{
    return m_window;
}

std::shared_ptr<Entity> Game::getPlayer()
{
    return m_player;
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
    // spawnEnemy();
    // ToDo: read variables...
    // ToDo: set framerate limit
}

void Game::run()
{
    // agregar que cuando pausamos algunos system no tienen que andar
    // y otros si como el render.
    const float dt = 1.0f /FPS_CAP;
    setGravity(true);
    setSimulation(SIMULATION);
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(m_window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        m_entities.update();

        if(!m_paused)
        {
            if( deltaTime >= maxPeriod )
            {
                lastFrame = currentFrame;
                sEnemySpawner();
            }
           // std::cout << 1/deltaTime << std::endl;
            sCollision();
            sMovement(dt);
        }

        sUserInput();
        sRender();

        glfwSwapBuffers(m_window);
        glfwPollEvents();
        m_currentFrame++;
    }
    glDeleteVertexArrays(1, &m_QuadVA);
    glDeleteBuffers(1, &m_QuadVB);
    glDeleteBuffers(1, &m_QuadIB);
    glfwTerminate();
}

void Game::setPaused()
{
    m_paused = !m_paused;
}

void Game::spawnPlayer()
{
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
    float ex = RandomNumber(-0.8f, 0.8f);
    float ey = RandomNumber(-0.8f, 0.8f);
    float vx = RandomNumber(-0.01f, 0.01f);
    float vy = RandomNumber(-0.01f, 0.01f);
    if (m_simulation)
    {
        ex = -1.75f;
        ey = 0.8f;
        vx = 0.005f;
        vy = 0.0f;
    }

    entity->cTransform = std::make_shared<CTransform>(glm::vec3(ex, ey,0.0f),
                                                     glm::vec3(vx, vy,0.0f), 0.0f);
    m_lastEnemySpawnTime = m_currentFrame;

    entity->cShape = std::make_shared<CShape>(ENEMY_SIZE, ENEMY_SIZE);
}

void Game::sEnemySpawner()
{
    // agregar par que cada x frames se meta uno.
    if ((m_currentFrame-m_lastEnemySpawnTime)%FPS_SPAWN==0)
    {
        spawnEnemy();
    }
}

void Game::sRender()
{
    if (rotate < 6.30f)
        rotate += 0.02f;
    else
        rotate = 0.0f;
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    uint32_t indexCount = 0;
    std::array<Vertex, MaxQuadsCount * 4> vertices;
    Vertex* buffer = vertices.data();

    glm::mat4 model(1.0f);
    m_ourShader->use();
    model =  scale(model, glm::vec3( 1.0f/ratio, 1.0f, 1.0f));
    for (auto e: m_entities.getEntities())
    {
        if(e->cTransform)
        {   Vec2 Size = Vec2(e->cShape->m_sizeX, e->cShape->m_sizeY);
            buffer = CreateQuad(buffer, e->cTransform->m_pos, Size);
            //QuadRotation(buffer, e->cTransform->m_pos, Size, rotate);
            indexCount += 6;
        }

    }
    // No camera system for now.
    glm::mat4 u_MPV =  model;
    m_ourShader->setMat4("u_MPV",  u_MPV);

    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glClear(GL_COLOR_BUFFER_BIT);

    glBindVertexArray(m_QuadVA);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
}

void Game::sMovement(float dt)
{
    for (auto e: m_entities.getEntities(Enemy))
    {
        std::shared_ptr<CTransform> enemy = e->cTransform;
        enemy->m_pos_old = enemy->m_pos;
        enemy->m_pos = enemy->m_pos_old + enemy->m_velocity + m_gravity*dt*dt;

        // Quad is center in the bottom left corner.
        // Ceiling
        if(enemy->m_pos[1]>1.0f-e->cShape->m_sizeY)
        {
            enemy->m_velocity[1] = -1.0f*enemy->m_velocity[1]*ABSORTION;
            enemy->m_pos[1] = enemy->m_pos[1] - 2*(enemy->m_pos[1] - (1.0f-e->cShape->m_sizeY));
        }

        // Floor
        if(enemy->m_pos[1]<-1.0f)
        {
            enemy->m_velocity[1] = -1.0f*enemy->m_velocity[1]*ABSORTION;
            enemy->m_pos[1] = enemy->m_pos[1] - 2*(enemy->m_pos[1] - (-1.0f));
        }

        // Left
        if(enemy->m_pos[0]<-1.0f*ratio)
        {
            enemy->m_velocity[0] = -1.0f*enemy->m_velocity[0]*ABSORTION;
            enemy->m_pos[0] = enemy->m_pos[0] - 2*(enemy->m_pos[0] - (-1.0f*ratio));
        }

        // Right
        if(enemy->m_pos[0]>1.0f*ratio-e->cShape->m_sizeX)
        {
            enemy->m_velocity[0] = -1.0f*enemy->m_velocity[0]*ABSORTION;
            enemy->m_pos[0] = enemy->m_pos[0] - 2*(enemy->m_pos[0] - (1.0f*ratio-e->cShape->m_sizeX));
        }

    }
    for (auto e: m_entities.getEntities(Bullet))
    {
        std::shared_ptr<CTransform> enemy = e->cTransform;
        enemy->m_pos_old = enemy->m_pos;
        enemy->m_pos = enemy->m_pos_old + enemy->m_velocity + m_gravity*dt*dt;

        // Quad is center in the bottom left corner.
        // Ceiling
        if(enemy->m_pos[1]>1.0f-e->cShape->m_sizeY)
        {
            enemy->m_velocity[1] = -1.0f*enemy->m_velocity[1]*ABSORTION;
            enemy->m_pos[1] = enemy->m_pos[1] - 2*(enemy->m_pos[1] - (1.0f-e->cShape->m_sizeY));
        }

        // Floor
        if(enemy->m_pos[1]<-1.0f)
        {
            enemy->m_velocity[1] = -1.0f*enemy->m_velocity[1]*ABSORTION;
            enemy->m_pos[1] = enemy->m_pos[1] - 2*(enemy->m_pos[1] - (-1.0f));
        }

        // Left
        if(enemy->m_pos[0]<-1.0f*ratio)
        {
            enemy->m_velocity[0] = -1.0f*enemy->m_velocity[0]*ABSORTION;
            enemy->m_pos[0] = enemy->m_pos[0] - 2*(enemy->m_pos[0] - (-1.0f*ratio));
        }

        // Right
        if(enemy->m_pos[0]>1.0f*ratio-e->cShape->m_sizeX)
        {
            enemy->m_velocity[0] = -1.0f*enemy->m_velocity[0]*ABSORTION;
            enemy->m_pos[0] = enemy->m_pos[0] - 2*(enemy->m_pos[0] - (1.0f*ratio-e->cShape->m_sizeX));
        }

    }
}

void Game::sRestart()
{
    m_player->cTransform->m_pos = m_player->cTransform->m_pos_init;
    m_player->cTransform->m_velocity = m_player->cTransform->m_velocity_init;
    m_player->cTransform->m_pos_old = m_player->cTransform->m_pos- m_player->cTransform->m_velocity;
}

void Game::sCollision()
{
    for ( auto e1: m_entities.getEntities())
    {
        for (auto e2: m_entities.getEntities())
        {
            if(e1!=e2)
            {
                if(sCheckCollision(e1, e2))
                {
                    if(m_player==e1 || m_player==e2)
                    {
                        sRestart();
                    }
                    // fijarse si es bullet enemy
                    else
                    {
                        float temp = e1->cTransform->m_velocity[0];
                        glm::vec3 diff_pos = e1->cTransform->m_pos - e2->cTransform->m_pos;
                        glm::vec3 diff_vel = e1->cTransform->m_velocity - e2->cTransform->m_velocity;
                        e1->cTransform->m_pos[0] -= e1->cTransform->m_velocity[0];
                        e1->cTransform->m_velocity[0] = e2->cTransform->m_velocity[0]*ABSORTION;
                        e2->cTransform->m_velocity[0] = temp*ABSORTION;

                        temp = e1->cTransform->m_velocity[1] ;
                        e1->cTransform->m_pos[1] -= e1->cTransform->m_velocity[1];
                        e1->cTransform->m_velocity[1] = e2->cTransform->m_velocity[1]*ABSORTION;
                        e2->cTransform->m_velocity[1] = temp*ABSORTION;

                        size_t loop = 0;
                        while(sCheckCollision(e1, e2)&& loop<10)
                        {
                            e1->cTransform->m_pos[0] -= e1->cTransform->m_velocity[0];
                            e2->cTransform->m_pos[0] -= e2->cTransform->m_velocity[0];
                            e1->cTransform->m_pos[1] -= e1->cTransform->m_velocity[1];
                            e2->cTransform->m_pos[1] -= e2->cTransform->m_velocity[1];
                            loop++;
                        }
                    }

                }
            }
        }
    }
}

bool Game::sCheckCollision(std::shared_ptr<Entity> one, std::shared_ptr<Entity> two)
{
    // collision x-axis?
    bool collisionX = one->cTransform->m_pos[0] + one->cShape->m_sizeX >= two->cTransform->m_pos[0]&&
        two->cTransform->m_pos[0] + two->cShape->m_sizeX >= one->cTransform->m_pos[0];
    // collision y-axis?
    bool collisionY = one->cTransform->m_pos[1] + one->cShape->m_sizeY >= two->cTransform->m_pos[1] &&
        two->cTransform->m_pos[1] + two->cShape->m_sizeY >= one->cTransform->m_pos[1];
    // collision only if on both axes
    bool res = collisionX && collisionY;
 //   std::cout << res << std::endl;
    return res;
}

void Game::sUserInput()
{
    std::shared_ptr<CTransform> player = m_player->cTransform;

    if(m_player->cInput->Keys[GLFW_KEY_W])
    {
        if(player->m_pos[1]<1.0f-m_player->cShape->m_sizeY)
            player->m_pos[1] += player->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_S])
    {
        if(player->m_pos[1]>-1.0f)
            player->m_pos[1] -= player->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_A])
    {
        if(player->m_pos[0]>-1.0f*ratio)
            player->m_pos[0] -= player->m_velocity[0];
    }
    if(m_player->cInput->Keys[GLFW_KEY_D])
    {
        if(player->m_pos[0]<1.0f*ratio-m_player->cShape->m_sizeX)
            player->m_pos[0] +=player->m_velocity[0];
    }
    if(m_player->cInput->Keys[GLFW_MOUSE_BUTTON_LEFT])
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window, &xpos, &ypos );
        xpos = xpos/SCREEN_WIDTH;
        ypos = ypos/SCREEN_HEIGHT;
        glm::vec2 mousePos = glm::vec2(xpos, ypos);
        spawnBullet(m_player, mousePos);
    }
}

void Game::setGravity(bool gravity)
{
    if(gravity)
        m_gravity = gravity_vec;
    else
        m_gravity = glm::vec3(0.0f, 0.0f, 0.0f);
}

void Game::setSimulation(bool simulation)
{
    m_simulation = simulation;
}
void Game::spawnSmallEnemies(std::shared_ptr<Entity> e)
{

}

void Game::spawnBullet(std::shared_ptr<Entity> e, const glm::vec2& target)
{
    auto entity = m_entities.addEntity(Bullet);

    glm::vec3 origin = e->cTransform->m_pos;
    glm::vec3 dir = origin - glm::vec3(target, 0.0f);
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(origin.x, origin.y ,0.0f),
                                                     dir, 0.0f);
    m_lastEnemySpawnTime = m_currentFrame;

    entity->cShape = std::make_shared<CShape>(BULLET_SIZE, BULLET_SIZE);
}

// Not used for now.
void Game::LoadConfig(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line, word;

    while (getline(stream, line))
    {
        std::stringstream ss(line);
        ss >> word;

        if ( word == "Player")
        {
            ss >> m_playerConfig.SR >> m_playerConfig.CR >> m_playerConfig.FR >>
                  m_playerConfig.FG >> m_playerConfig.FB >> m_playerConfig.OR >>
                  m_playerConfig.OG >> m_playerConfig.OB >> m_playerConfig.OT >>
                  m_playerConfig.V;
            ss >> m_playerConfig.S;
        }
        else if (word == "Enemy" )
        {
            ss >> m_enemyConfig.SR   >> m_enemyConfig.CR   >> m_enemyConfig.OR >>
                  m_enemyConfig.OG   >> m_enemyConfig.OB   >> m_enemyConfig.OT >>
                  m_enemyConfig.VMIN >> m_enemyConfig.VMAX >> m_enemyConfig.L >>
                  m_enemyConfig.SI;
            ss >> m_enemyConfig.SMIN >> m_enemyConfig.SMAX;
        }
        else if (word == "Bullet")
        {
            ss >> m_bulletConfig.SR >> m_bulletConfig.CR  >> m_bulletConfig.FR >>
                  m_bulletConfig.FG >> m_bulletConfig.FB  >> m_bulletConfig.OR >>
                  m_bulletConfig.OG >> m_bulletConfig.OB  >> m_bulletConfig.OT >>
                  m_bulletConfig.V  >> m_bulletConfig.L;
            ss >> m_bulletConfig.S;
        }
    }
}


