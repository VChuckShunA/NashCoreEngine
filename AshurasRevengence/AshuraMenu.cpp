#include "../Action.h"
#include "AshuraMenu.h"
#include "AshuraLevel.h"

AshuraMenu::AshuraMenu(GameEngine* gameEngine) : Scene(gameEngine)
{
    init();
}
void AshuraMenu::init()
{
    registerAction(sf::Keyboard::Space, "PLAY");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "ASHURA'S REVENGENCE";
    int titleSize = 60;

    m_menuText.setString(m_title);
    m_menuText.setFont(m_game->assets().getFont("Mario"));
    m_menuText.setCharacterSize(titleSize);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(
        float(m_game->window().getSize().x) / 2.0f - float(titleSize * (m_title.length() + 1)) / 2.0f,
        float(titleSize * 3)
    );


    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");

    m_menuStrings.emplace_back("Press Space to Play");

    for (int i = 0; i < m_menuStrings.size(); i++) {
        sf::Text text(m_menuStrings[i], m_game->assets().getFont("Mario"), 26);
        if (i != m_selectedMenuIndex) {
            text.setFillColor(sf::Color::Black);
        }
        text.setPosition(
            float(m_game->window().getSize().x) / 2.0f - float(26 * (m_menuStrings[i].length() + 1)) / 2.0f,
            m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1)
        );
        m_menuItems.push_back(text);
    }

    m_levelPaths.emplace_back("AshuraLevel.txt");
}

void AshuraMenu::update()
{
    sRender();
}

void AshuraMenu::onEnd()
{
    m_game->quit();
}

void AshuraMenu::sDoAction(const Action& action)
{
    if (action.type() == "START") {
      
        
        if (action.name() == "PLAY") {
            m_game->changeScene("PLAY", std::make_shared<AshuraLevel>(m_game, m_levelPaths[m_selectedMenuIndex]));
        }
        else if (action.name() == "QUIT") {
            onEnd();
        }
    }
}



void AshuraMenu::sRender()
{ // set menu background
    m_game->window().clear(sf::Color(216, 176, 64));

    // draw title
    m_game->window().draw(m_menuText);

    // draw menu items
    for (int i = 0; i < m_menuStrings.size(); i++) {
        if (i != m_selectedMenuIndex) {
            m_menuItems[i].setFillColor(sf::Color::Black);
        }
        else {
            m_menuItems[i].setFillColor(sf::Color::White);
        }

        m_menuItems[i].setPosition(
            float(m_game->window().getSize().x) / 2.0f - float(26 * (m_menuStrings[i].length() + 1)) / 2.0f,
            m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1)
        );
        m_game->window().draw(m_menuItems[i]);
    }

    // draw help
    sf::Text help(" WASD: Movement Space: Shoot Q: Toggle Quadtrees ", m_game->assets().getFont("Mario"), 26);
    help.setFillColor(sf::Color::Black);
    help.setPosition(
        float(m_game->window().getSize().x) / 2.0f - float(26 * (help.getString().getSize() + 1)) / 2.0f,
        float(m_game->window().getSize().y) - 30.0f * 2.0f
    );
    m_game->window().draw(help);
}
