#pragma once
#include "Scene.h"
#include "GameEngine.h"

#include <vector>
#include <SFML/Graphics.hpp>
class Scene_Menu : public Scene {
protected:
    std::string m_title;
    std::vector<std::string> m_menuStrings;

    sf::Text m_menuText;
    std::vector<sf::Text> m_menuItems;

    std::vector<std::string> m_levelPaths;
    size_t m_selectedMenuIndex = 0;

    void init();

    void update() override;

    void onEnd() override;

    void sDoAction(const Action& action) override;

public:
    explicit Scene_Menu(GameEngine* gameEngine = nullptr);

    void sRender() override;
};

