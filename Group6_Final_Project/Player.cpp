#include "Player.h"

void Player::initVariables()
{
	this->movementSpeed = 8.f;
	this->hpMax = 10;
	this->hp = hpMax;
}

void Player::initTexture()
{
	this->texture.loadFromFile("Assets/Textures/Player/Shark4.png");
}

void Player::initSprite()
{
	this->sprite.setTexture(texture);
	this->sprite.setScale(1.5, 1.5);
}

Player::Player(float x, float y)
{
	this->sprite.setPosition(x, y);

	this->initVariables();
	this->initTexture();
	this->initSprite();

}

Player::~Player()
{

}

//Accessors
const sf::Sprite& Player::getSprite() const
{
	return this->sprite;
}

const int& Player::getHp() const
{
	return this->hp;
}

const int& Player::getHpMax() const
{
	return this->hpMax;
}

//Functions
void Player::takeDamage(const int damage)
{
	if (this->hp > 0)
		this->hp -= damage;

	if (this->hp < 0)
		this->hp = 0;
}

void Player::gainHealth(const int health)
{
	if (this->hp < this->hpMax)
		this->hp += health;

	if (this->hp > this->hpMax)
		this->hp = this->hpMax;
}

void Player::updateInput()
{
	//Keyboard input
	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->sprite.move(-this->movementSpeed, 0.f);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->sprite.move(this->movementSpeed, 0.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->sprite.move(0.f, -this->movementSpeed);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->sprite.move(0.f, this->movementSpeed);
	}
}

void Player::updateWindowBoundsCollision(const sf::RenderTarget* target) {
	sf::FloatRect playerBounds = this->sprite.getGlobalBounds();
	sf::Vector2u windowSize = target->getSize();

	// Left
	if (playerBounds.left < 0.f) {
		this->sprite.setPosition(0.f, playerBounds.top);
	}
	// Right
	if (playerBounds.left + playerBounds.width > windowSize.x) {
		this->sprite.setPosition(windowSize.x - playerBounds.width, playerBounds.top);
	}
	// Top
	if (playerBounds.top < 0.f) {
		this->sprite.setPosition(playerBounds.left, 0.f);
	}
	// Bottom
	if (playerBounds.top + playerBounds.height > windowSize.y) {
		this->sprite.setPosition(playerBounds.left, windowSize.y - playerBounds.height);
	}
}

void Player::update(const sf::RenderTarget* target)
{
	this->updateInput();

	//Window bounds collision
	this->updateWindowBoundsCollision(target);
}

void Player::render(sf::RenderTarget* target)
{
	target->draw(this->sprite);
}
