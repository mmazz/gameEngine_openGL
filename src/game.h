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
#include <ft2build.h>
#include FT_FREETYPE_H

//opengl?

struct WorldConfig  { int W, H, FL, FS; bool SIM, GRAV; };
struct FontConfig   { std::string F; int S, R, G, B; };
struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { float SR; int CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN, SMAX; } ;
struct BulletConfig { float SR; int CR, FR, FG, FB, OR, OG, OB, OT, V, L ; float S; };

class Game
{
    GLFWwindow* m_window;
    std::shared_ptr<Entity> m_player;
    unsigned int m_QuadVB, m_QuadVA, m_QuadIB;
    unsigned int m_VBO_text, m_VAO_text;
    EntityManager m_entities;
   // font m_font;
   // text m_text;
    float m_ratio;
    Shader* m_ourShader;
    Shader* m_ourShader_text;
    WorldConfig  m_worldConfig;
    FontConfig   m_fontConfig;
    PlayerConfig m_playerConfig;
    EnemyConfig  m_enemyConfig;
    BulletConfig m_bulletConfig;
    int          m_score = 0;
    int          m_currentFrame = 0;
    int          m_lastEnemySpawnTime = 0;
    bool         m_paused = false;
    bool         m_running = true;
    bool         m_simulation = true;

    glm::vec3 m_gravity = glm::vec3(0.0f, -0.1f, 0.0f);
    void init();
    void sRestart();

    void sMovement(float dt);
    void sUserInput();
    void sLifeSpan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void sResolveCollision(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
    void sResolveCollisionBullet(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);
    void sWallConstrains(std::shared_ptr<Entity> enemy);
    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const glm::vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);
    void LoadConfig(const std::string& filepath);
    bool sCheckCollision(std::shared_ptr<Entity> one, std::shared_ptr<Entity> two);

    void RenderText(Shader* shader, std::string text, float x, float y, float scale, glm::vec3 color);
    glm::vec3 sManifoldCollision(std::shared_ptr<Entity> e1, std::shared_ptr<Entity> e2);

public:
    Game(const std::string & config, const char* vertexPath, const char* fragmentPath);
    ~Game();
    GLFWwindow* getWindow();
    std::shared_ptr<Entity> getPlayer();
    void run();
    void setPaused();
    void setGravity(bool gravity);
    void setSimulation(bool simulation);
    void setWinSize(int width, int height);
};

