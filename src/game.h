#pragma once
#include "entity.h"
#include "entityManager.h"
//opengl?

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; } ;
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L ; float S; };

class Game
{
    //glRenderWindow m_window;
    EntityManager m_entities;
   // font m_font;
   // text m_text;
    PlayerConfig m_playerConfig;
    EnemyConfig  m_enemyConfig;
    BulletConfig m_bulletConfig;
    int          m_score = 0;
    int          m_currentFrame = 0;
    int          m_lastenemySpawnTime = 0;
    bool         m_paused = false;
    bool         m_running = true;

    std::shared_ptr<Entity> m_player;
    void init(const std::string& config);
    void setPaused(bool paused);

    void sMovement();
    void sUserInput();
    void sLifeSpan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const glm::vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
};


