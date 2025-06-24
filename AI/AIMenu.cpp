#include "AIMenu.h"
#include "../Action.h"
#include "AIPlayroom.h"
AIMenu::AIMenu(GameEngine* gameEngine) : Scene(gameEngine)
{
	init();
}
void AIMenu::init()
{
    registerAction(sf::Keyboard::Space, "PLAY");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "boustrophedon Behaviour Trees";
    int titleSize = 40;

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
    m_menuStrings.emplace_back("                                            This is an AI Simulation created within SFML utilizing Behaviour Trees!");
    m_menuStrings.emplace_back("                                            It's not a game per se, but rather a game that plays itself!\n");

    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("                    Press Space to Run Simulation");

    for (int i = 0; i < m_menuStrings.size(); i++) {
        sf::Text text(m_menuStrings[i], m_game->assets().getFont("Mario"), 15);
        if (i != m_selectedMenuIndex) {
            text.setFillColor(sf::Color::Black);
        }
        text.setCharacterSize(18);
        m_menuItems.push_back(text);
    }

    m_levelPaths.emplace_back("level1.txt");
    m_levelPaths.emplace_back("AshuraLevel.txt");
    m_levelPaths.emplace_back("level2.txt");
    m_levelPaths.emplace_back("level3.txt");
}

void AIMenu::update()
{
    sRender();
}

void AIMenu::onEnd()
{
    m_game->quit();
}

void AIMenu::sDoAction(const Action& action)
{
    if (action.type() == "START") {


        if (action.name() == "PLAY") {
            m_game->changeScene("PLAY", std::make_shared<AIPlayroom>(m_game, m_levelPaths[1]));
        }
        else if (action.name() == "QUIT") {
            onEnd();
        }
    }
}



void AIMenu::sRender()
{// set menu background
    m_game->window().clear(sf::Color::White);

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
    sf::Text help("                           K: Set Health to 22 C: Toggle Collisions  G: Toggle Grid ", m_game->assets().getFont("Mario"), 20);
    help.setFillColor(sf::Color::Black);
    help.setPosition(
        float(m_game->window().getSize().x) / 2.0f - float(26 * (help.getString().getSize() + 1)) / 2.0f,
        float(m_game->window().getSize().y) - 30.0f * 2.0f
    );
    help.setCharacterSize(20);
    m_game->window().draw(help);
}
