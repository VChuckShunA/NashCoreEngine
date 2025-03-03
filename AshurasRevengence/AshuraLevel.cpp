#include "AshuraLevel.h"
#include "../Action.h"
#include "../SceneMenu.h"
#include "../Vec2.h"
#include "../Physics.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include "WFC/WFC.h"
#include <random>
#include "../Quadtrees/Quadtree.h"
void AshuraLevel::init(const std::string& levelPath)
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");   // Toggle drawing (T)extures
    registerAction(sf::Keyboard::C, "TOGGLE_COLLISION"); // Toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");      // Toggle drawing (G)rid

    // Register all other gameplay Actions
    registerAction(sf::Keyboard::W, "JUMP");
    registerAction(sf::Keyboard::S, "DOWN");
    registerAction(sf::Keyboard::A, "LEFT");
    registerAction(sf::Keyboard::D, "RIGHT");
    registerAction(sf::Keyboard::Space, "SHOOT");
    registerAction(sf::Keyboard::Q, "QUADTREE");

    m_gridText.setCharacterSize(12);
    // m_gridText.setFont(m_game->assets().getFont("Mario"));
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    loadLevel(levelPath);
}

Vec2 AshuraLevel::gridToMidPixel(float gridX, float gridY, const std::shared_ptr<Entity>& entity)
{
    // This function takes in a grid (x,y) position and an Entity
  // Return a vec2 indicating where the CENTER position of the Entity should be
  // You must use the Entity's Animation size to position it correctly
  // The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
  // The bottom-left corner of the Animation should align with the bottom left of the grid cell

    auto entitySize = entity->getComponent<CAnimation>().animation.getSize();
    // vec2 offset = m_gridSize - entitySize;
    return {
            gridX * m_gridSize.x + entitySize.x / 2.0f,
            height() - gridY * m_gridSize.y - entitySize.y / 2.0f
    };
    //    return {
    //            gridX * m_gridSize.x + m_gridSize.x / 2.0f - offset.x,
    //            height() - gridY * m_gridSize.y - m_gridSize.y / 2.0f + offset.y
    //    };
}

void AshuraLevel::loadLevel(const std::string& fileName)
{  // reset the entity manager every time we load a level
    m_entityManager = EntityManager();
    quadtree= new Quadtree(sf::FloatRect(0, 0, width(), height()));
    WFC wfc = WFC(*this);
    SpawnEnemies();
    // read in the level file and add the appropriate entities
    // use the PlayerConfig struct m_playerConfig to store player properties
    // this struct is defined at the top of Scene_Play.h
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "AshuraLevel::loadLevel could not load " << fileName << " file.\n";
        exit(-1);
    }

    std::string entityType;
   
    while (file >> entityType) {
        if (entityType == "Tile") {
            std::string animationName;
            float gridX, gridY;
            file >> animationName >> gridX >> gridY;

            auto tile = m_entityManager.addEntity("tile");
            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            tile->addComponent<CTransform>(
                gridToMidPixel(gridX, gridY, tile),
                Vec2(0, 0),
                Vec2(1, 1),
                0
            );
            tile->addComponent<CBoundingBox>(m_gridSize);

        }
        else if (entityType == "Dec") {
            std::string animationName;
            float gridX, gridY;
            file >> animationName >> gridX >> gridY;

            auto dec = m_entityManager.addEntity("dec");
            dec->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
            dec->addComponent<CTransform>(
                gridToMidPixel(gridX, gridY, dec),
                Vec2(0, 0),
                Vec2(1, 1),
                0
            );

        }
        else if (entityType == "Player") {
            file >> m_playerConfig.X >> m_playerConfig.Y
                >> m_playerConfig.CX >> m_playerConfig.CY
                >> m_playerConfig.SPEED
                >> m_playerConfig.JUMP
                >> m_playerConfig.MAX_SPEED
                >> m_playerConfig.GRAVITY
                >> m_playerConfig.WEAPON;
            spawnPlayer();

        }
        else {
            std::cerr << "Unknown entity type " << entityType << "\n";
            // exit(-1);
        }


    }

}

void AshuraLevel::spawnPlayer()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entityManager.addEntity("player");
    m_player->addComponent<CAnimation>(m_game->assets().getAnimation("Ashura"), true);
    m_player->addComponent<CTransform>(
        gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player),
        Vec2(m_playerConfig.SPEED, 0),
        Vec2(1, 1),
        0
    );
    m_player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));

    // be sure to add the remaining components to the player
    m_player->addComponent<CInput>();
   // m_player->addComponent<CState>("stand");
    m_player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    m_player->getComponent<CInput>().canJump = true;
    m_player->getComponent<CGravity>().gravity = 0;
    m_player->getComponent<CTransform>().velocity.y = 0;
                // collision resolution

   
}

void AshuraLevel::SpawnEnemies()
{
    auto enemy = m_entityManager.addEntity("enemy");

    int randomX = generateRandomNumber(20, 23);
    int randomy = generateRandomNumber(0, 12);
    int randSpeed = generateRandomNumber(0, 1);
    float dir = -1.0f;
    enemy->addComponent<CAnimation>(m_game->assets().getAnimation("Star"), true);
    enemy->addComponent<CTransform>(
        gridToMidPixel(randomX, randomy, enemy),
        Vec2(0, 0),
        Vec2(1, 1),
        0
    );
    enemy->addComponent<CBoundingBox>(m_gridSize);
    enemy->addComponent<CTransform>(
        enemy->getComponent<CTransform>().pos,
        Vec2(dir * randSpeed, 0),
        enemy->getComponent<CTransform>().scale,
        0
    );
    enemy->addComponent<CLifespan>(1600, m_currentFrame);
    enemies.push_back(enemy.get());
}

int AshuraLevel::generateRandomNumber(int min, int max)
{
    // Create a random device and a random engine
    std::random_device rd;
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_int_distribution<> dis(min, max);  // Uniform distribution in the range [min, max]

    return dis(gen);  // Generate and return the random number
}

void AshuraLevel::spawnBullet(const std::shared_ptr<Entity>& entity)
{ // this should spawn a bullet at the given entity, going in the direction the entity is facing
    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.WEAPON), true);
    // vec2(30,-3) is a tweak so that bullet starts at the end of gun; it is determined experimentally
    float dir = 1.0f;
    if (entity->getComponent<CTransform>().scale.x < 0) dir = -1.0;
    bullet->addComponent<CTransform>(
        entity->getComponent<CTransform>().pos + Vec2(45,-20) ,
        Vec2(dir * 2 * m_playerConfig.SPEED, 0),
        // vec2(5 * entity->getComponent<CTransform>().scale.x, 0),
        entity->getComponent<CTransform>().scale,
        0
    );
    bullet->addComponent<CLifespan>(90, m_currentFrame);
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
    bullets.push_back(bullet.get());
}

void AshuraLevel::sMovement()
{
    auto& transform = m_player->getComponent<CTransform>();
    auto& input = m_player->getComponent<CInput>();

    transform.velocity.x = 0;

    if (input.left) {
        transform.velocity.x = -m_playerConfig.SPEED;     
    }
    else if (input.right) {
        transform.velocity.x = m_playerConfig.SPEED;   
    }

    transform.velocity.y = 0;  // Reset y-velocity each frame
    if (input.up) {
        transform.velocity.y = -m_playerConfig.SPEED; // Move up
    }
    if (input.down) {
        transform.velocity.y = m_playerConfig.SPEED;  // Move down
    }
    //    else if (m_player->getComponent<CTransform>().velocity.y <= 0) {
    //        m_player->getComponent<CTransform>().velocity.y = 0;
    //    }

    if (m_player->getComponent<CInput>().shoot) {
        if (m_player->getComponent<CInput>().canShoot) {
            spawnBullet(m_player);
            m_player->getComponent<CInput>().canShoot = false;
        }
    }
    else {
        m_player->getComponent<CInput>().canShoot = true;
    }

    // update all entities positions
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->hasComponent<CGravity>()) {
            Vec2& v = entity->getComponent<CTransform>().velocity;
            v.y += entity->getComponent<CGravity>().gravity;
            if (v.y > m_playerConfig.MAX_SPEED) {
                v.y = m_playerConfig.MAX_SPEED;
            }
        }
        entity->getComponent<CTransform>().prevPos = entity->getComponent<CTransform>().pos;
        entity->getComponent<CTransform>().pos += entity->getComponent<CTransform>().velocity;
    }
}

void AshuraLevel::sLifespan()
{ // Check lifespan of entities that have them, and destroy them if they go over
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->hasComponent<CLifespan>()) {
            if (entity->getComponent<CAnimation>().animation.getName() == "CoinSpin") {
                if (entity->getComponent<CAnimation>().animation.hasEnded()) {
                    entity->destroy();
                }
            }
            else {
                auto& eLife = entity->getComponent<CLifespan>();
                if (m_currentFrame - eLife.frameCreated >= eLife.lifespan) {
                    if (entity->tag() == "enemy")
                    {
                        auto it = std::find(enemies.begin(), enemies.end(), entity.get());
                        if (it != enemies.end()) {
                            enemies.erase(it);  
                        }
                    }
                    else if (entity->tag() == "bullet")
                    {
                        auto it = std::find(bullets.begin(), bullets.end(), entity.get());
                        if (it != bullets.end()) {
                            bullets.erase(it);  
                        }
                    }
                    entity->destroy();
                }
            }
        }
    }

    // control bullet quantity, be spawned every 10 frames
    for (const auto& entity : m_entityManager.getEntities("bullet")) {
        auto& bulletLife = entity->getComponent<CLifespan>();
        if (m_currentFrame - bulletLife.frameCreated == 5) {
            m_player->getComponent<CInput>().canShoot = true;
        }
    }
}

void AshuraLevel::sCollision()
{ 
    bool useQuadTrees = true;
    if (!useQuadTrees)
    {
        for (const auto& bullet : m_entityManager.getEntities("bullet")) {

            for (const auto& enemy : m_entityManager.getEntities("enemy")) {
                // check bullet and tile side collide
                Vec2 overlap = Physics::GetOverlap(bullet, enemy);
                Vec2 pOverlap = Physics::GetPreviousOverlap(bullet, enemy);
                if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                    if (0 <= overlap.x && pOverlap.x <= 0) {
                        if (enemy->getComponent<CAnimation>().animation.getName() == "Star") {
                            spawnBrickDebris(enemy);
                        }
                        bullet->destroy();
                    }
                }
            }
        }

        for (const auto& player : m_entityManager.getEntities("player")) {

            for (const auto& enemy : m_entityManager.getEntities("enemy")) {
                // check bullet and tile side collide
                Vec2 overlap = Physics::GetOverlap(player, enemy);
                Vec2 pOverlap = Physics::GetPreviousOverlap(player, enemy);
                if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                    if (0 <= overlap.x && pOverlap.x <= 0) {
                        if (enemy->getComponent<CAnimation>().animation.getName() == "Star") {
                            spawnBrickDebris(enemy);
                        }
                        player->destroy();
                        //Return to Menu
                    }
                }
            }
        }
    }
    else
    {
        quadtree->clear();
        for (size_t i = 0; i < bullets.size(); ++i) {
            auto& bullet = bullets[i]; // Access bullet
            Point* p = new Point("Bullet", getGlobalBounds(bullet), i);
            quadtree->insert(p);
        }
        for (size_t i = 0; i < enemies.size(); ++i) {
            auto& enemy = enemies[i]; // Access enemy
            Point* p = new Point("Enemy", getGlobalBounds(enemy), i);
            quadtree->insert(p);
        }
        
        Point* playerPoint = new Point("Player",
            getGlobalBounds(m_player.get()),-1);
        quadtree->insert(playerPoint);
       
        for (size_t i = 0; i < bullets.size(); ++i) {
        
            std::vector<Point*> points = quadtree->queryRange(getGlobalBounds(bullets[i]));
            for (int j = 0; j < points.size(); j++)
            {
                if (points[j]->tag != "Bullet")
                {
                  
                    if (points[j]->tag == "Enemy" && getGlobalBounds(enemies[points[j]->index]).intersects(getGlobalBounds(bullets[i])))
                    {
                        if (enemies[points[j]->index]->getComponent<CAnimation>().animation.getName() == "Star") {
                            enemies[points[j]->index]->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Explosion");
                            enemies[points[j]->index]->addComponent<CLifespan>(10, m_currentFrame);
                            bullets[i]->destroy();

                            auto it = std::find(bullets.begin(), bullets.end(), bullets[i]);
                            if (it != bullets.end()) {
                                bullets.erase(it);
                            }
                        }
                       
                    } 
                }
               
                
            }
            
        }

        for (size_t i = 0; i < enemies.size(); ++i) {

            std::vector<Point*> points = quadtree->queryRange(getGlobalBounds(enemies[i]));
            for (int j = 0; j < points.size(); j++)
            {
                if (points[j]->tag != "Enemy")
                {
                    if (points[j]->tag == "Player" && getGlobalBounds(m_player.get()).intersects(getGlobalBounds(enemies[i])))
                    {
                       
                        m_player->destroy();
                        //Return to Menu
                    }
                }
               
            }

        }
        
        //for (int i = 0; i < bullets.size(); i++) {
        //    std::vector<Point*> points = quadTree.queryRange(bullets[i]->getGlobalBounds());
        //    for (int j = 0; j < points.size(); j++) {
        //        //We dont want collision detection with bullets and itself
        //        if (points[j]->tag != "Bullet") {
        //            if (points[j]->tag == "Enemy" && bullets[i]->playerShoot && enemies[points[j]->index]->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
        //                enemies[points[j]->index]->TakeDamage(bullets[i]->damageAmount);
        //                bullets[i]->toRemove = true;
        //            }
        //            else if (points[j]->tag == "Player" && !bullets[i]->playerShoot && player->getGlobalBounds().intersects(bullets[i]->getGlobalBounds())) {
        //                player->TakeDamage(bullets[i]->damageAmount);
        //                bullets[i]->toRemove = true;
        //            }


        //        }

        //    }
        //}
        //for (int i = 0; i < bullets.size(); i++) {
        //    if (bullets[i]->toRemove) {
        //        bullets.erase(bullets.begin() + i);
        //    }
        //}
        //for (int i = 0; i < enemies.size(); i++) {
        //    if (enemies[i]->toRemove) {
        //        enemies.erase(enemies.begin() + i);
        //    }
        //}

    }
   
   

    // Check to see if the player has fallen down a hole (y > height())
    if (m_player->getComponent<CTransform>().pos.y > height()) {
        m_player->getComponent<CTransform>().pos = gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, m_player);
    }

    // Clamp map movement to map
    if (m_player->getComponent<CTransform>().pos.x < m_player->getComponent<CBoundingBox>().size.x / 2.0f) {
        m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CBoundingBox>().size.x / 2.0f;
    }
    if (m_player->getComponent<CTransform>().pos.x > width() - m_player->getComponent<CBoundingBox>().size.x / 2.0f) {
        m_player->getComponent<CTransform>().pos.x = width() - m_player->getComponent<CBoundingBox>().size.x / 2.0f;
    }

    if (m_player->getComponent<CTransform>().pos.y < m_player->getComponent<CBoundingBox>().size.y / 2.0f) {
        m_player->getComponent<CTransform>().pos.y = m_player->getComponent<CBoundingBox>().size.y / 2.0f;
    }

    if (m_player->getComponent<CTransform>().pos.y > height() - m_player->getComponent<CBoundingBox>().size.y / 2.0f) {
        m_player->getComponent<CTransform>().pos.y = height() - m_player->getComponent<CBoundingBox>().size.y / 2.0f;
    }
}

void AshuraLevel::sAnimation()
{ // Complete the Animation class code first
    // Set the animation of the player based on its CState component
    // check player state
   /* if (m_player->getComponent<CTransform>().velocity.y != 0) {
        m_player->getComponent<CInput>().canJump = false;
        if (m_player->getComponent<CInput>().shoot) {
            changePlayerStateTo("airshoot");
        }
    }
    else {
        if (m_player->getComponent<CTransform>().velocity.x != 0) {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo("runshoot");
            }
            else {
                changePlayerStateTo("run");
            }
        }
        else {
            if (m_player->getComponent<CInput>().shoot) {
                changePlayerStateTo("standshoot");
            }
            else {
                changePlayerStateTo("stand");
            }
        }
    }

    // change player animation
    if (m_player->getComponent<CState>().changeAnimation) {
        std::string animationName;
        if (m_player->getComponent<CState>().state == "stand") {
            animationName = "Stand";
        }
        else if (m_player->getComponent<CState>().state == "air") {
            animationName = "Jump";
        }
        else if (m_player->getComponent<CState>().state == "run") {
            animationName = "Run";
        }
        else if (m_player->getComponent<CState>().state == "standshoot") {
            animationName = "StandShoot";
        }
        else if (m_player->getComponent<CState>().state == "airshoot") {
            animationName = "AirShoot";
        }
        else if (m_player->getComponent<CState>().state == "runshoot") {
            animationName = "RunShoot";
        }
        // std::cout << "Ivan: getAnimation " << animationName << "\n";
        m_player->addComponent<CAnimation>(m_game->assets().getAnimation(animationName), true);
    }
    */
    for (const auto& entity : m_entityManager.getEntities()) {
        if (entity->getComponent<CAnimation>().animation.hasEnded() && !entity->getComponent<CAnimation>().repeat) {
            entity->destroy();
        }
        if (entity->hasComponent<CAnimation>()) {
            entity->getComponent<CAnimation>().animation.update();
        }
    }
    // call entity->getComponent<CAnimation>().animation.update()
    // if the animation is not repeated, and it has ended, destroy the entity
}

void AshuraLevel::sRender()
{// color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

  

    // draw all Entity textures / animations
    if (m_drawTextures) {
        for (const auto& e : m_entityManager.getEntities()) {
            auto& transform = e->getComponent<CTransform>();
            if (e->hasComponent<CAnimation>()) {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_drawCollision) {
        for (const auto& e : m_entityManager.getEntities()) {
            if (e->hasComponent<CBoundingBox>()) {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }
    if(q_drawQuadtrees)
        quadtree->show(m_game->window());
    // draw the grid so that can easily debug
    if (m_drawGrid) {
        float leftX = float(m_game->window().getView().getCenter().x) - width() / 2.0f;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - float((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += float(m_gridSize.y)) {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += float(m_gridSize.x)) {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string yCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
}

void AshuraLevel::sDoAction(const Action& action)
{
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "QUADTREE") { q_drawQuadtrees = !q_drawQuadtrees; }

        else if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = true;
            //if (m_player->getComponent<CInput>().canJump) { m_player->getComponent<CInput>().up = true; }
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = true;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = true;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = true;
        }
        else if (action.name() == "SHOOT") {
                m_player->getComponent<CInput>().shoot = true;
          
        }
    }
    else if (action.type() == "END") {
        if (action.name() == "JUMP") {
            m_player->getComponent<CInput>().up = false;
        }
        else if (action.name() == "DOWN") {
            m_player->getComponent<CInput>().down = false;
        }
        else if (action.name() == "LEFT") {
            m_player->getComponent<CInput>().left = false;
        }
        else if (action.name() == "RIGHT") {
            m_player->getComponent<CInput>().right = false;
        }
        else if (action.name() == "SHOOT") {
            m_player->getComponent<CInput>().shoot = false;
        }
    }
}

void AshuraLevel::onEnd()
{  // when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void AshuraLevel::changePlayerStateTo(const std::string& state)
{
    auto& prev = m_player->getComponent<CState>().previousState;
    if (prev != state) {
        prev = m_player->getComponent<CState>().state;
        m_player->getComponent<CState>().state = state;
        m_player->getComponent<CState>().changeAnimation = true;
    }
    else {
        m_player->getComponent<CState>().changeAnimation = false;
    }
}

void AshuraLevel::spawnCoinSpin(const std::shared_ptr<Entity>& tile)
{
    auto coin = m_entityManager.addEntity("coinspin");
    coin->addComponent<CAnimation>(
        m_game->assets().getAnimation("CoinSpin"),
        false
    );
    coin->addComponent<CTransform>(
        Vec2(
            tile->getComponent<CTransform>().pos.x,
            tile->getComponent<CTransform>().pos.y - m_gridSize.y
        ),
        Vec2(0, 0),
        tile->getComponent<CTransform>().scale,
        0
    );
    coin->addComponent<CLifespan>(30, m_currentFrame);
}

void AshuraLevel::spawnBrickDebris(const std::shared_ptr<Entity>& tile)
{
    tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Explosion");
    // tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("BrickDebris");
    tile->addComponent<CLifespan>(10, m_currentFrame);
}

AshuraLevel::AshuraLevel(GameEngine* gameEngine, const std::string& levelPath) : Scene(gameEngine), m_levelPath(levelPath) {
    init(levelPath);
}

void AshuraLevel::update()
{
    m_entityManager.update();

    // implement pause functionality
    if (!m_paused) {
   // for (int i = 0; i < 50; i++)
   // {

            SpawnEnemies();
      //  }
        
    
        sMovement();
        sLifespan();
        sCollision();
        for (const auto& enemy : m_entityManager.getEntities("enemy")) {
            enemy->getComponent<CTransform>().angle++;
            enemy->getComponent<CAnimation>().animation.getSprite().setRotation(enemy->getComponent<CTransform>().angle);
        }
        m_currentFrame++;
    }
    sAnimation();
    sRender();
}

sf::FloatRect AshuraLevel::getGlobalBounds(Entity* entity)
{
    return sf::FloatRect(entity->getComponent<CTransform>().pos.x - entity->getComponent<CBoundingBox>().halfSize.x,
        entity->getComponent<CTransform>().pos.y - entity->getComponent<CBoundingBox>().halfSize.y,
        entity->getComponent<CBoundingBox>().size.x, entity->getComponent<CBoundingBox>().size.y);
}
