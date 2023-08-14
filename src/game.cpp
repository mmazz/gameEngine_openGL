#include "game.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1280;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 720;




/*float vertices[] = {
        // positions          // colors    re coords
     0.5f,  0.5f,// 0.0f,   1.0f, 0.0f, 0.0f,  // top right
     0.5f, -0.5f,// 0.0f,   0.0f, 1.0f, 0.0f,  // bottom right
    -0.5f, -0.5f,// 0.0f,   0.0f, 0.0f, 1.0f,  // bottom left
    -0.5f,  0.5f//, 0.0f,   1.0f, 1.0f, 0.0f  // top left
};*/
struct Vec2
{
    float x, y;
};

struct Vertex
{
    Vec2 Position;
};

static std::array<Vertex, 4> CreateQuad(float x, float y)
{
    float size = 1.0f;
    Vertex v0;
    v0.Position = {x, y};
    Vertex v1;
    v1.Position = {x+size, y};
    Vertex v2;
    v2.Position = {x+size, y+size};
    Vertex v3;
    v3.Position = {x, y+size};
    return { v0, v1, v2, v3 };
}
unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3  // second triangle
};

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
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    auto q0 = CreateQuad(-1.5f, -0.5f);
    auto q1 = CreateQuad(0.5f, -0.5f);

    Vertex vertices[8];
    memcpy(vertices, q0.data(), q0.size() * sizeof(Vertex));
    memcpy(vertices + q0.size(), q1.data(), q1.size() * sizeof(Vertex));
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);    // position attribute
    // el primero es el componente que estamos seteando, en este caso el primero osea 0
    // despues es la cantidad de elementos que tiene, tenemos 3 posiciones
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
                                                                     //
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
        // calculate delta time
        // --------------------
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

    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
}

void Game::setPaused(bool paused)
{

}

void Game::spawnPlayer()
{
    // falta agregar demas propiedades del config
    auto entity = m_entities.addEntity(Player);
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(1.0f, 0.0f,1.0f),
                                                     glm::vec3(0.01f, 0.01f,0.0f), 0.0f);
//   entity->cShape = std::make_shared<CShape>();
//
//    // falta meter inputs
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
    for (int i=0; i<1024; i++)
    {
        m_player->cInput->Keys[i] = false;
    }


}
// Tieene que hacer draw de todo...
void Game::sRender()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // render the triangle
    m_ourShader->use();
    glBindVertexArray(m_VAO);
    glm::mat4 model(1.0f);
    glm::mat4 translate(1.0f);
    glm::mat4 scale(1.0f);
    //glm::mat4 translate = glm::translate(model, glm::vec3(0.0f,0.0f, 1.0f));
    for (auto e: m_entities.getEntities())
    {
        model = glm::mat4(1.0f);
        translate = glm::translate(model, e->cTransform->m_pos);
        scale = glm::scale(model, glm::vec3(0.5f,0.5f,1.0f));
        model = scale * translate;
        m_ourShader->setMat4("u_MPV", model);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        // 6=cantidad de indices
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
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


void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity(Enemy);
//    float ex = rand() % window.Width;
 //   float ey = rand() % window.Height;
    entity->cTransform = std::make_shared<CTransform>(glm::vec3(1.0f, 0.0f,1.0f),
                                                     glm::vec3(0.01f, 0.05f,0.0f), 0.0f);
//    //falta meter las variables del config
//    m_lastEnemySpawnTime = m_currentFrame;

}

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
        if(m_player->cTransform->m_pos[1]<SCREEN_HEIGHT)
            m_player->cTransform->m_pos[1] += m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_S])
    {
        if(m_player->cTransform->m_pos[1]>0.0f)
            m_player->cTransform->m_pos[1] -= m_player->cTransform->m_velocity[1];
    }
    if(m_player->cInput->Keys[GLFW_KEY_A])
    {
        if(m_player->cTransform->m_pos[0]>0.0f)
            m_player->cTransform->m_pos[0] -= m_player->cTransform->m_velocity[0];
    }
    if(m_player->cInput->Keys[GLFW_KEY_D])
    {
        if(m_player->cTransform->m_pos[0]<SCREEN_WIDTH)
            m_player->cTransform->m_pos[0] +=m_player->cTransform->m_velocity[0];
    }
    for (auto e: m_entities.getEntities(Enemy))
    {
        if(e->cTransform->m_pos[1]>2)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[1]<0.0f)
            e->cTransform->m_velocity[1] = -1*e->cTransform->m_velocity[1];

        if(e->cTransform->m_pos[0]<0.0f)
            e->cTransform->m_velocity[0] = -1*e->cTransform->m_velocity[0];

        if(e->cTransform->m_pos[0]>2)
            e->cTransform->m_velocity[0] = -1*e->cTransform->m_velocity[0];

        e->cTransform->m_pos += e->cTransform->m_velocity;
        std::cout << e->cTransform->m_pos[1] << std::endl;
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


