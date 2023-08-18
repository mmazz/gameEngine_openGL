#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <array>

#include "entity.h"
#include "entityManager.h"
#include "components.h"
#include "shader.h"

//opengl?

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; } ;
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L ; float S; };

class Game
{
    GLFWwindow* m_window;
    std::shared_ptr<Entity> m_player;
    unsigned int m_QuadVB, m_QuadVA, m_QuadIB;
    EntityManager m_entities;
   // font m_font;
   // text m_text;
    Shader* m_ourShader;
    PlayerConfig m_playerConfig;
    EnemyConfig  m_enemyConfig;
    BulletConfig m_bulletConfig;
    int          m_score = 0;
    int          m_currentFrame = 0;
    int          m_lastEnemySpawnTime = 0;
    bool         m_paused = false;
    bool         m_running = true;
    void init(const std::string& config);

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
    void LoadConfig(const std::string& filepath);

public:
    Game(const std::string & config, const char* vertexPath, const char* fragmentPath);
    ~Game();
    GLFWwindow* getWindow();
    std::shared_ptr<Entity> getPlayer();
    void run();
    void setPaused();
};

