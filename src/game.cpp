#include "game.h"

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;

Game::Game(const std::string& config)
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    init(config);
}

void Game::init(const std::string& path)
{

    //read variables...
    // create window
    // set framerate limit
    //
    spawnPlayer();
}

void Game::run()
{

    // agregar que cuando pausamos algunos system no tienen que andar
    // y otros si como el render.
    //

    while (!glfwWindowShouldClose(m_window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();

        glfwPollEvents();

        m_entities.update();

        if(!m_paused)
        {
            sEnemySpawner();
            //sMovement();
            sCollision();
            sUserInput();
        }
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        sRender();

        glfwSwapBuffers(m_window);
        m_currentFrame++;
    }

    glfwTerminate();
    while(m_running)
    {


    }
}

void Game::setPaused(bool paused)
{

}

void Game::spawnPlayer()
{
    // falta agregar demas propiedades del config
    auto entity = m_entities.addEntity(Player);

//    entity->cTransform = std::make_shared<CTransform>(glm::vec2(200.0f, 200.0f),
//            glm::vec2(1.0f, 1.0f), 0.0f);
// //   entity->cShape = std::make_shared<CShape>(args);
//
//    // falta meter inputs
//    entity->cInput = std::make_shared<CInput>();
//
    m_player = entity;
}

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
void Game::sMovement(std::vector<Entity>& entities)
{
 //   for (auto& e : entities)
 //   {
 //       if (e.cTransform)
 //           e.cTransform->pos += e.cTransform->velocity;
 //   }
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
// Tieene que hacer draw de todo...
void Game::sRender()
{
//    for (auto& e : m_entities.getEntities())
//    {
        // si no tiene, es que tiene un nullptr y va a ser falso y no entra
  //      if (e.cTransform && e.cShape)
 //       {
  //          e.cShape->shape.setPosition(e.cTransform->pos);
  //          window.draw(e.cShape->shape);
   //     }
 //   }
}

void Game::sUserInput()
{
//    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//        glfwSetWindowShouldClose(window, true);
//    if (key >= 0 && key < 1024)
//    {
//        if (action == GLFW_PRESS)
//            MatiDo.Keys[key] = true;
//        else if (action == GLFW_RELEASE)
//            MatiDo.Keys[key] = false;
//    }
}
