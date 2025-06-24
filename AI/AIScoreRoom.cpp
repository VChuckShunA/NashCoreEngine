#include "AIScoreRoom.h"
#include "../Action.h"
#include "AIPlayroom.h"
void AIScoreRoom::init()
{
    registerAction(sf::Keyboard::Space, "PLAY");
    registerAction(sf::Keyboard::Escape, "QUIT");

    m_title = "Simulation Terminated";
    int titleSize = 50;

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
    m_menuStrings.emplace_back("AI Score : " + std::to_string(SCORE));
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");
    m_menuStrings.emplace_back("\n");

    m_menuStrings.emplace_back("Press Space to Restart the simulation");

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

    m_levelPaths.emplace_back("level1.txt");
}

void AIScoreRoom::update()
{
    sRender();
}

void AIScoreRoom::onEnd()
{
    m_game->quit();
}

void AIScoreRoom::sDoAction(const Action& action)
{
    if (action.type() == "START") {


        if (action.name() == "PLAY") {
            m_game->changeScene("PLAY", std::make_shared<AIPlayroom>(m_game, m_levelPaths[m_selectedMenuIndex]));
        }
        else if (action.name() == "QUIT") {
            onEnd();
        }
    }
}

AIScoreRoom::AIScoreRoom(GameEngine* gameEngine, int Score) : Scene(gameEngine), SCORE(Score)
{
	init();
}

void AIScoreRoom::sRender()
{ // set menu background
    m_game->window().clear(sf::Color::White);

    // draw title
    m_game->window().draw(m_menuText);

    // draw menu items
    for (int i = 0; i < m_menuStrings.size(); i++) {
        if (i != m_selectedMenuIndex) {
            m_menuItems[i].setFillColor(sf::Color::Black);
        }
        else {
            m_menuItems[i].setFillColor(sf::Color::Black);
        }

        m_menuItems[i].setPosition(
            float(m_game->window().getSize().x) / 2.0f - float(26 * (m_menuStrings[i].length() + 1)) / 2.0f,
            m_menuText.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1)
        );
        m_game->window().draw(m_menuItems[i]);
    }

    sf::View view = m_game->window().getView();
    view.setCenter(m_game->window().getSize().x/2, m_game->window().getSize().y / 2);//(float(m_game->window().getSize().x), 0));
    m_game->window().setView(view);
}
