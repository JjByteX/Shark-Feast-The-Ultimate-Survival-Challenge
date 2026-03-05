#include "SwagBall.h"
#include <iostream>

void SwagBall::initTexture(const sf::RenderWindow& window)
{
    // Use shared_ptr to manage texture lifetime
    this->texture = std::make_shared<sf::Texture>();

    std::string texturePath;
    switch (this->type)
    {
    case DEFAULT:
        texturePath = "Assets/Textures/Creatures/Fish.png";
        break;
    case DAMAGING:
        texturePath = "Assets/Textures/Bombs/bomb.png";
        break;
    case HEALING:
        texturePath = "Assets/Textures/Creatures/Jellyfish.png";
        break;
    }

    if (!this->texture->loadFromFile(texturePath))
    {
        std::cerr << "ERROR::SWAGBALL::INITTEXTURE::Could not load texture from " << texturePath << std::endl;
        return;
    }
}

void SwagBall::initSprite(const sf::RenderWindow& window)
{
    // Set the sprite's texture using the shared_ptr
    this->sprite.setTexture(*this->texture, true);

    // Randomize position within window bounds
    float spriteWidth = this->sprite.getGlobalBounds().width;
    float spriteHeight = this->sprite.getGlobalBounds().height;

    this->sprite.setPosition(
        sf::Vector2f(
            static_cast<float>(rand() % (window.getSize().x - static_cast<int>(spriteWidth))),
            static_cast<float>(rand() % (window.getSize().y - static_cast<int>(spriteHeight)))
        )
    );

    // Optional: Randomize scale for variety
    float randomScale = (rand() % 50 + 50) / 100.f; // Scale between 0.5 and 1.0
    this->sprite.setScale(randomScale, randomScale);
}

SwagBall::SwagBall(const sf::RenderWindow& window, int type)
    : type(type)
{
    // Different vertical speeds for different ball types
    switch (type)
    {
    case DEFAULT:
        verticalSpeed = 1.0f; // Slower for default balls
        break;
    case HEALING:
        verticalSpeed = 2.0f; // Faster for healing balls
        break;
    case DAMAGING:
        verticalSpeed = 0.0f; // Stationary damaging balls
        break;
    }

    initTexture(window);
    initSprite(window);
}


const sf::Sprite& SwagBall::getShape() const
{
    return this->sprite;
}

const int& SwagBall::getType() const
{
    return this->type;
}

void SwagBall::update(const sf::RenderWindow& window)
{
    // Move upward for default and healing balls
    if (type == DEFAULT || type == HEALING)
    {
        sprite.move(0, -verticalSpeed);
    }
}

void SwagBall::render(sf::RenderTarget& target)
{
    target.draw(this->sprite);
}