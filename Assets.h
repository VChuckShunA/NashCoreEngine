#pragma once

#include "Animation.h"

#include <map>

class Assets {
    std::map<std::string, sf::Texture> m_textureMap;
    std::map<std::string, Animation> m_animationMap;
    std::map<std::string, sf::Font> m_fontMap;

public:
    Assets();

    void loadFromFile(const std::string& path);

    void addTexture(const std::string& name, const std::string& path);

    void addAnimation(const std::string& name, const Animation& animation);

    void addFont(const std::string& name, const std::string& path);

    [[nodiscard]] const sf::Texture& getTexture(const std::string& name) const;

    [[nodiscard]] const Animation& getAnimation(const std::string& name) const;

    [[nodiscard]] const sf::Font& getFont(const std::string& name) const;
};