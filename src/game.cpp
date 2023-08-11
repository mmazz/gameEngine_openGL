#include "game.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;




float vertices[] = {
    // positions         // colors
     0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
     0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
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

    spawnPlayer();
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
   entity->cTransform = std::make_shared<CTransform>(glm::vec2(200.0f, 200.0f),
           glm::vec2(1.0f, 1.0f), 0.0f);
   entity->cShape = std::make_shared<CShape>(Triangle, glm::vec2(200.0f, 200.0f), glm::vec2(100.0f, 100.0f),  glm::vec3(1.0f), glm::vec2( 0.0f, 0.0f));
//
//    // falta meter inputs
    entity->cInput = std::make_shared<CInput>();
    m_player = entity;
    for (int i=0; i<1024; i++)
    {
        m_player->cInput->Keys[i] = false;
    }
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

}
// Tieene que hacer draw de todo...
void Game::sRender()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // render the triangle
    m_ourShader->use();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
    float ex = 100.0;
    float ey = 50.0;
//    entity->cTransform = std::make_shared<CTransform>(glm::vec2(ex, ey),
//            glm::vec2(1.0f, 1.0f), 0.0f);
//    int args = 1; //hacer...
//    entity->cShape = std::make_shared<CShape>(args);
//
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
        m_player->cTransform->m_pos +=m_player->cTransform->m_velocity;
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


