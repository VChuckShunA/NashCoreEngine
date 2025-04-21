#include "AIPlayroom.h"
#include "../Action.h"
#include "../SceneMenu.h"
#include "../Vec2.h"
#include "../Physics.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include<numbers>
AIPlayroom::AIPlayroom(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {

    init(levelPath);



}

void AIPlayroom::init(const std::string& levelPath) {
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
    registerAction(sf::Keyboard::K, "MoveAgent");

    m_gridText.setCharacterSize(12);
    // m_gridText.setFont(m_game->assets().getFont("Mario"));
    m_gridText.setFont(m_game->assets().getFont("Tech"));

    navmesh.initializeNavMesh();
    //Spawn AI
    AIAgent = m_entityManager.addEntity("agent");
    AIAgent->addComponent<CAnimation>(m_game->assets().getAnimation("GreenAgent"), true);
    AIAgent->addComponent<CTransform>(
        gridToMidPixel(0,11, AIAgent),
        Vec2(3, 0),
        Vec2(1, 1),
        0
    );
    AIAgent->addComponent<CBoundingBox>(Vec2(64, 64));
    AIAgent->addComponent<CVision>();
    
    
    std::shared_ptr<Entity> AIAgent2;
    AIAgent2 = m_entityManager.addEntity("player");
    AIAgent2->addComponent<CAnimation>(m_game->assets().getAnimation("GreenAgent"), true);
    AIAgent2->addComponent<CTransform>(
        gridToMidPixel(2, 6, AIAgent2),
        Vec2(3, 0),
        Vec2(1, 1),
        0
    );
    AIAgent2->addComponent<CBoundingBox>(Vec2(64, 64));
   
   GreenAgent greenAgent(AIAgent, this);
   agents.push_back(std::make_unique<GreenAgent>(AIAgent, this));

   
}


Vec2 AIPlayroom::gridToMidPixel(float gridX, float gridY, const std::shared_ptr<Entity>& entity) {
    // This function takes in a grid (x,y) position and an Entity
    // Return a vec2 indicating where the CENTER position of the Entity should be
    // You must use the Entity's Animation size to position it correctly
    // The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    // The bottom-left corner of the Animation should align with the bottom left of the grid cell

    auto entitySize = entity->getComponent<CAnimation>().animation.getSize();
 
    return {
            gridX * m_gridSize.x + entitySize.x / 2.0f,
            height() - gridY * m_gridSize.y - entitySize.y / 2.0f
    };
}



void AIPlayroom::spawnBullet(const std::shared_ptr<Entity>& entity) {
    //spawning bullet
    auto bullet = m_entityManager.addEntity("bullet");
    bullet->addComponent<CAnimation>(m_game->assets().getAnimation("Buster"), true);

    //Setting bullet transforms
    bullet->addComponent<CTransform>(
                entity->getComponent<CTransform>().pos ,    //position
                Vec2(0, 0),                                 //previous position
                Vec2{1.0,1.0},                              //scale
                entity->getComponent<CTransform>().angle    //angle
    ); 
  //  float angle = 90 * (std::numbers::pi / 180.0f);
     // Retrieve the position and angle from the entity
    auto& target = m_entityManager.getEntities("player").at(0).get()->getComponent<CTransform>();

    // shoot straight (at your own angle)
    auto& entityTransform = entity->getComponent<CTransform>();
    Vec2 position = entityTransform.pos;
    float angleDegrees = entityTransform.angle;
    float angleRadians = angleDegrees * (std::numbers::pi / 180.0f);


    //Turn towards Target
   // float deltaX = target.pos.x - entity->getComponent<CTransform>().pos.x;
    //float deltaY = target.pos.y - entity->getComponent<CTransform>().pos.y;
    //float angleRadians = std::atan2(deltaY, deltaX); // Angle in radians
    //float angleDegrees = angleRadians * (180.0f / std::numbers::pi); // Convert to degrees if needed

    // Convert angle from degrees to radians
   // float angleRadians = 90 * (std::numbers::pi / 180.0f);

    // Define bullet speed
    float speed = 2.0f;
    bullet->getComponent<CTransform>().velocity.x = cos(angleRadians) * speed;
    bullet->getComponent<CTransform>().velocity.y = sin(angleRadians) * speed;
    bullet->addComponent<CLifespan>(90, m_currentFrame);
    bullet->addComponent<CBoundingBox>(bullet->getComponent<CAnimation>().animation.getSize());
}

void AIPlayroom::update() {
    m_entityManager.update();

    // implement pause functionality
    if (!m_paused) {
        sMovement();
        sLifespan();
        sCollision();
        sVisionCone();
        RunBehaviourTrees();
        sVisionCone();
        m_currentFrame++;
    }
    sRender();
}

Vec2 AIPlayroom::positionToGridCordinates(const std::shared_ptr<Entity>& entity)
{
    return { (entity->getComponent<CTransform>().pos.x / 64) ,
        ((m_game->window().getSize().y - (entity->getComponent<CTransform>().pos.y - 1)) / 64) };
  
}

void AIPlayroom::MoveEntity(const std::shared_ptr<Entity>& entity, std::vector<Vec2>& path)
{
    int AISpeed = 1;
    bool destinationReached = false;
    bool up = false, down = false, left = false, right = false;
    float angleToWaypoint;
    Vec2 A = Vec2((int)positionToGridCordinates(entity).x, (int)positionToGridCordinates(entity).y);
    Vec2 B= Vec2(path.front().x, path.front().y);
 
     //0=right,90=down,180 =left, 270=up
    Vec2 distanceBetween;
    if (!path.empty()) {
        if (!destinationReached)
        {
            distanceBetween = Vec2(abs(entity->getComponent<CTransform>().pos.x - gridToMidPixel(path.front().x, path.front().y, entity).x), abs(entity->getComponent<CTransform>().pos.y - gridToMidPixel(path.front().x, path.front().y, entity).y));
            if (entity->getComponent<CTransform>().pos == Vec2(gridToMidPixel(path.back().x, path.back().y, entity)))
            {
                destinationReached=true;
            }
            
           /* if (Vec2((int)positionToGridCordinates(entity).x, (int)positionToGridCordinates(entity).y) == Vec2(path.front().x, path.front().y))
            {
                path.erase(path.begin());
            }*/
            /*if (Vec2(entity->getComponent<CTransform>().pos.x, entity->getComponent<CTransform>().pos.y) == Vec2(gridToMidPixel(path.front().x,path.front().y,entity)))
            {
                path.erase(path.begin());
            }*/
            if (distanceBetween.x<5&&distanceBetween.y<5)
            {
                path.erase(path.begin());
            }
            //TODO: Find a cleaner a way to do this
            if (entity->getComponent<CTransform>().pos.x < gridToMidPixel(path.front().x, path.front().y, entity).x)
            {
                //move Left
                entity->getComponent<CTransform>().pos.x= entity->getComponent<CTransform>().pos.x+AISpeed;
                left = false;
                right = true;
            }if (entity->getComponent<CTransform>().pos.x > gridToMidPixel(path.front().x, path.front().y, entity).x)
            {
                //move Right
                entity->getComponent<CTransform>().pos.x=entity->getComponent<CTransform>().pos.x-AISpeed;
                left = true;
                right = false;
            }
            if (entity->getComponent<CTransform>().pos.y < gridToMidPixel(path.front().x, path.front().y, entity).y)
            {
                //move Down
                entity->getComponent<CTransform>().pos.y = entity->getComponent<CTransform>().pos.y+AISpeed;
                down = true;
                up = false;
               
            }if (entity->getComponent<CTransform>().pos.y > gridToMidPixel(path.front().x, path.front().y, entity).y)
            {
                //move Up
                entity->getComponent<CTransform>().pos.y = entity->getComponent<CTransform>().pos.y-AISpeed;
                up = true;
                down = false;
            }
            //0=right,90=down,180 =left, 270=up

            if (up && left)
            {
                //entity->getComponent<CTransform>().angle = 225;
                steer(entity,225);
            }
            if (up && right)
            {
                //entity->getComponent<CTransform>().angle = 315;
                steer(entity, 315);
            }
            if (down && left)
            {
               // entity->getComponent<CTransform>().angle = 135;
                steer(entity, 135);
            }
            if (down && right)
            {
               // entity->getComponent<CTransform>().angle = 45;
                steer(entity, 45);
            }
            if (up)
            {
                //entity->getComponent<CTransform>().angle = 270;
                steer(entity, 270);
            }
            if (down)
            {
                //entity->getComponent<CTransform>().angle = 90;
                steer(entity, 90);
            }
            if (left)
            {
               // entity->getComponent<CTransform>().angle = 180;
                steer(entity, 180);
            }
            if (right)
            {
               // entity->getComponent<CTransform>().angle = 0;
                steer(entity, 0);
            }
        }
        //entity->getComponent<CTransform>().pos = Vec2(gridToMidPixel(path.front().x,path.front().y,entity));
       // path.erase(path.begin());
    }
}

void AIPlayroom::sVisionCone()
{
    for (auto& enemy : m_entityManager.getEntities("agent")) {
        if (!enemy->hasComponent<CVision>() || !enemy->hasComponent<CTransform>()) continue;

        auto& vision = enemy->getComponent<CVision>();
        auto& transform = enemy->getComponent<CTransform>();

        auto& player = m_entityManager.getEntities("player")[0]; // Assuming single player entity
        auto& playerTransform = player->getComponent<CTransform>();

        Vec2 toPlayer = playerTransform.pos - transform.pos;
        float distance = toPlayer.length();
        Vec2  eye = transform.pos;
        float rad = transform.angle * (std::numbers::pi / 180.0f);
        Vec2  dir = { std::cos(rad), std::sin(rad) };

        // Check if player is within vision range
        if (distance > vision.visionRange) {
            vision.seesPlayer = false;
            vision.Target = nullptr;
            continue;
        }

        // Normalize vector to player
        toPlayer.normalize();
        Vec2 dirToPlayer = toPlayer;

        // Get enemy forward direction (assuming enemy faces right initially)
        Vec2 enemyForward = Vec2(cos(transform.angle), sin(transform.angle));

        // Dot product to check FOV
        float dot = enemyForward.dot(dirToPlayer);
        float cosHalfFOV = cos(vision.fovAngle * 0.5f * (std::numbers::pi / 180.0f));

        if (dot < cosHalfFOV) {
           // vision.seesPlayer = false;
            vision.Target = nullptr;
            continue;
        }

        
        
        if (vision.IsTargetInFOV(eye,dir,playerTransform.pos)) {
            vision.seesPlayer = true;
            vision.Target = enemy;
            std::cout << "SAW THE PLAYER" << std::endl;
        }
        else if (!vision.IsTargetInFOV(eye, dir, playerTransform.pos)) {
            vision.seesPlayer = false;
            vision.Target = nullptr;
            std::cout << "NO PLAYER" << std::endl;
        }
    }
}

void AIPlayroom::drawVisionCone()
{
   // if (!(AIAgent->getComponent<CVision>())) return;

    auto& vision = AIAgent->getComponent<CVision>();
    auto& transform = AIAgent->getComponent<CTransform>();

    sf::VertexArray visionCone(sf::TrianglesFan, 12);
    visionCone[0].position = sf::Vector2f(transform.pos.x, transform.pos.y);
    visionCone[0].color = sf::Color(255, 255, 0, 100);
    if(vision.seesPlayer){ visionCone[0].color = sf::Color(255, 0, 0, 100); }

    for (int i = 0; i <= 10; i++) {
        float angle = transform.angle - vision.fovAngle * 0.5f + (vision.fovAngle / 10.0f) * i;
        float rad = angle * (std::numbers::pi / 180.0f);
        Vec2 point = transform.pos + Vec2(cos(rad), sin(rad)) * vision.visionRange;

        visionCone[i + 1].position = sf::Vector2f(point.x, point.y);
        visionCone[i + 1].color = sf::Color(255, 255, 0, 100);
        if (vision.seesPlayer) { visionCone[i + 1].color = sf::Color(255, 0, 0, 100); }
    }

    m_game->window().draw(visionCone);
}

void AIPlayroom::steer(const std::shared_ptr<Entity>& entity, float targetAngle)
{
    if (entity->getComponent<CTransform>().angle==targetAngle) return;

    float turnAngle = fmod(targetAngle - entity->getComponent<CTransform>().angle + 360, 360); // Normalize difference

    if (turnAngle<180)
    {
        //clockwise
        entity->getComponent<CTransform>().angle++;
    }
    if (turnAngle > 180)
    {
        //counter clock wise
        entity->getComponent<CTransform>().angle--;
    }
    entity->getComponent<CTransform>().angle = fmod(entity->getComponent<CTransform>().angle + 360, 360);
}

void AIPlayroom::aimAndShoot(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Entity>& target)
{
    bool up = false, down = false, left = false, right = false;
    //TODO: Find a cleaner a way to do this
    if (entity->getComponent<CTransform>().pos.x < target->getComponent<CTransform>().pos.x)
    {
        //move Left
        left = false;
        right = true;
    }if (entity->getComponent<CTransform>().pos.x > target->getComponent<CTransform>().pos.x)
    {
        //move Right
        left = true;
        right = false;
    }
    if (entity->getComponent<CTransform>().pos.y < target->getComponent<CTransform>().pos.y)
    {
        //move Down
        down = true;
        up = false;

    }if (entity->getComponent<CTransform>().pos.y > target->getComponent<CTransform>().pos.y)
    {
        //move Up
        up = true;
        down = false;
    }
    //0=right,90=down,180 =left, 270=up

    if (up && left)
    {
        //entity->getComponent<CTransform>().angle = 225;
        steer(entity, 225);
    }
    if (up && right)
    {
        //entity->getComponent<CTransform>().angle = 315;
        steer(entity, 315);
    }
    if (down && left)
    {
        // entity->getComponent<CTransform>().angle = 135;
        steer(entity, 135);
    }
    if (down && right)
    {
        // entity->getComponent<CTransform>().angle = 45;
        steer(entity, 45);
    }
    if (up)
    {
        //entity->getComponent<CTransform>().angle = 270;
        steer(entity, 270);
    }
    if (down)
    {
        //entity->getComponent<CTransform>().angle = 90;
        steer(entity, 90);
    }
    if (left)
    {
        // entity->getComponent<CTransform>().angle = 180;
        steer(entity, 180);
    }
    if (right)
    {
        // entity->getComponent<CTransform>().angle = 0;
        steer(entity, 0);
    }
}

void AIPlayroom::RunBehaviourTrees()
{
    for (auto& agent : agents) {
        agent->update();
    }
}


void AIPlayroom::sLifespan() {
    // Check lifespan of entities that have them, and destroy them if they go over
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
                    entity->destroy();
                }
            }
        }
    }

    // control bullet quantity, be spawned every 10 frames
    for (const auto& entity : m_entityManager.getEntities("bullet")) {
        auto& bulletLife = entity->getComponent<CLifespan>();
        if (m_currentFrame - bulletLife.frameCreated == 20) {
           // AIAgent->getComponent<CInput>().canShoot = true;
        }
    }
}

void AIPlayroom::sMovement()
{

    // update all entities positions
  for (const auto& entity : m_entityManager.getEntities("bullet")) {
    auto& transform = entity->getComponent<CTransform>();
    transform.prevPos = transform.pos;
    transform.pos.x += transform.velocity.x;
    transform.pos.y += transform.velocity.y;
}
}

void AIPlayroom::sCollision() {
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // Implement Physics::GetOverlap() function, use it inside this function

    // Implement bullet/tile collisions
    // Destroy the tile if it has a Brick animation
    for (const auto& bullet : m_entityManager.getEntities("bullet")) {
        for (const auto& tile : m_entityManager.getEntities("tile")) {
            // check bullet and tile side collide
            Vec2 overlap = Physics::GetOverlap(bullet, tile);
            Vec2 pOverlap = Physics::GetPreviousOverlap(bullet, tile);
            if (0 < overlap.y && -m_gridSize.x < overlap.x) {
                if (0 <= overlap.x && pOverlap.x <= 0) {
                    if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                        spawnBrickDebris(tile);
                    }
                    bullet->destroy();
                }
            }
        }
    }

    // Implement player/tile collisions and resolutions
    // Update the CState component of the player to store whether
    // it is currently on the ground or in the air. This will be
    // used by the Animation system
    // reset gravity
   // m_player->getComponent<CGravity>().gravity = m_playerConfig.GRAVITY;
    for (const auto& tile : m_entityManager.getEntities("tile")) {
        Vec2 overlap = Physics::GetOverlap(AIAgent, tile);
        Vec2 pOverlap = Physics::GetPreviousOverlap(AIAgent, tile);
        // check if player is in air
        // check tiles being below player
        float dy = tile->getComponent<CTransform>().pos.y - AIAgent->getComponent<CTransform>().pos.y;
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy > 0) {
            if (0 <= overlap.y && pOverlap.y <= 0) {
                // stand on tile
                AIAgent->getComponent<CInput>().canJump = true;
                AIAgent->getComponent<CGravity>().gravity = 0;
                AIAgent->getComponent<CTransform>().velocity.y = 0;
                // collision resolution
                AIAgent->getComponent<CTransform>().pos.y -= overlap.y;
            }
        }
        // check if player hits the tile from the bottom
        if (0 < overlap.x && -m_gridSize.y < overlap.y && dy < 0) {
            if (0 <= overlap.y && pOverlap.y <= 0) {
                AIAgent->getComponent<CTransform>().pos.y += overlap.y;
                AIAgent->getComponent<CTransform>().velocity.y = 0;
                if (tile->getComponent<CAnimation>().animation.getName() == "Question") {
                    tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("QuestionHit");
                    spawnCoinSpin(tile);
                }
                if (tile->getComponent<CAnimation>().animation.getName() == "Brick") {
                    spawnBrickDebris(tile);
                }
            }
        }
        // check player and tile side collide
        float dx = tile->getComponent<CTransform>().pos.x - AIAgent->getComponent<CTransform>().pos.x;
        if (0 < overlap.y && -m_gridSize.x < overlap.x) {
            if (0 <= overlap.x && pOverlap.x <= 0) {
                if (dx > 0) {
                    // tile is right of player
                    AIAgent->getComponent<CTransform>().pos.x -= overlap.x;
                }
                else {
                    // tile is left of player
                    AIAgent->getComponent<CTransform>().pos.x += overlap.x;
                }
            }
        }
    }

   
}

void AIPlayroom::sDoAction(const Action& action) {
    if (action.type() == "START") {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION") { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused); }
        else if (action.name() == "QUIT") { onEnd(); }
        else if (action.name() == "MoveAgent") { MoveEntity(AIAgent, path); }

      
    }
 
}


void AIPlayroom::onEnd() {
    // when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void AIPlayroom::sRender() {
    // color the background darker, so you know that the game is paused
    if (!m_paused) {
        m_game->window().clear(sf::Color(54, 54, 54));
    }
    else {
        m_game->window().clear(sf::Color(4, 4, 4));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
   // auto& pPos = m_player->getComponent<CTransform>().pos;
   // float windowCenterX = std::max(float(m_game->window().getSize().x) / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    //view.setCenter(windowCenterX, float(m_game->window().getSize().y) - view.getCenter().y);
    m_game->window().setView(view);

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

    navmesh.DrawPath(m_game->window());
    drawVisionCone();
    // draw the grid 
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
                std::string yCell = std::to_string(((int)y / (int)m_gridSize.y));
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }


    
}

void AIPlayroom::changePlayerStateTo(const std::string& state) {
    auto& prev = AIAgent->getComponent<CState>().previousState;
    if (prev != state) {
        prev = AIAgent->getComponent<CState>().state;
        AIAgent->getComponent<CState>().state = state;
        AIAgent->getComponent<CState>().changeAnimation = true;
    }
    else {
        AIAgent->getComponent<CState>().changeAnimation = false;
    }
}

void AIPlayroom::spawnBrickDebris(const std::shared_ptr<Entity>& tile) {
    tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("Explosion");
    // tile->getComponent<CAnimation>().animation = m_game->assets().getAnimation("BrickDebris");
    tile->addComponent<CLifespan>(10, m_currentFrame);
}

void AIPlayroom::spawnCoinSpin(const std::shared_ptr<Entity>& tile) {
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
