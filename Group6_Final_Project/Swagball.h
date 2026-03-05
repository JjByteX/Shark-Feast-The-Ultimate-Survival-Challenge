#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

class SwagBall
{
public:
    enum BallType { DEFAULT, DAMAGING, HEALING };

private:
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
    int type;
    float verticalSpeed; // Add vertical speed for healing balls

    void initTexture(const sf::RenderWindow& window);
    void initSprite(const sf::RenderWindow& window);

public:
    SwagBall(const sf::RenderWindow& window, int type);
    virtual ~SwagBall() = default;

    const sf::Sprite& getShape() const;
    const int& getType() const;

    void update(const sf::RenderWindow& window);
    void render(sf::RenderTarget& target);
};
