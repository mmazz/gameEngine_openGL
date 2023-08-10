#include "game.h"
#include <iostream>
#include <memory>

Game::Game(const std::string& config)
{
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
    while(m_running)
    {
        m_entities.update();

        if(!m_paused)
        {
            sEnemySpawner();
            sMovement();
            sCollision();
            sUserInput();
        }
        sRender();

        m_currentFrame++;
    }
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
 //   entity->cShape = std::make_shared<CShape>(args);

    // falta meter inputs
    entity->cInput = std::make_shared<CInput>();

    m_player = entity;
}

void Game::spawnEnemy()
{
    auto entity = m_entities.addEntity(Enemy);
//    float ex = rand() % window.Width;
 //   float ey = rand() % window.Height;
    float ex = 100.0;
    float ey = 50.0;
    entity->cTransform = std::make_shared<CTransform>(glm::vec2(ex, ey),
            glm::vec2(1.0f, 1.0f), 0.0f);
    int args = 1; //hacer...
    entity->cShape = std::make_shared<CShape>(args);

    //falta meter las variables del config
    m_lastEnemySpawnTime = m_currentFrame;

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
    for (auto& e : entities)
    {
        if (e.cTransform)
            e.cTransform->pos += e.cTransform->velocity;
    }
}

void Game::sCollision()
{
}

void Game::sEnemySpawner()
{
    // agregar par que cada x frames se meta uno.
    if (m_currentFrame - m_lastEnemySpawnTime)
        spawnEnemy();
}


// RGBA el A es de alpha de transparencia.
// Requere que tenga CTransform y CShape
//
//
// Tieene que hacer draw de todo...
void Game::sRender()
{
    for (auto& e : m_entities.getEntities())
    {
        // si no tiene, es que tiene un nullptr y va a ser falso y no entra
  //      if (e.cTransform && e.cShape)
 //       {
  //          e.cShape->shape.setPosition(e.cTransform->pos);
  //          window.draw(e.cShape->shape);
   //     }
    }
}

void Game::sUserInput()
{

}
