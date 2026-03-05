#pragma once
#include <iostream>
#include <ctime>
#include <vector>
#include <sstream>
#include <random>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include "Player.h"
#include "SwagBall.h"

class Game
{
private:
    // Enum for ball types to match SwagBall
    enum SwagBallTypes { DEFAULT, DAMAGING, HEALING };

    // Variables
    sf::VideoMode videoMode;
    sf::RenderWindow* window;
    sf::Event sfmlEvent;
    sf::Time time;
    sf::Clock clock;

    // Game state
    bool endGame;
    sf::Vector2i mousePos;

    // Player
    Player player;

    // Game mechanics
    int points;
    float spawnTimerMax;
    float spawnTimer;
    int maxSwagBalls;

    // UI elements
    sf::Font font;
    sf::Text guiText;
    sf::Text endGameText;

    // Sound
    sf::Music music;

    // Game objects
    std::vector<SwagBall> swagBalls;

    // Private initialization functions
    void initVariables();
    void initWindow();
    void initFonts();
    void initText();
    void initMusic();

    // Camera view
    sf::View gameView;
    sf::Vector2f cameraOffset;

    // Camera methods
    void updateCamera();

    // Add method to calculate camera movement
    sf::Vector2f calculateCameraMovement();

    // Background
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

    // New method declaration
    void initBackground();

public:
    // Constructors and Destructors
    Game();
    ~Game();

    // Accessors
    const bool& getEndGame() const;

    // Game loop functions
    const bool running() const;
    void pollEvents();
    void renderMainMenu(sf::RenderWindow& window);
    void updateMousePosition();
    void spawnSwagBalls();
    const int randBallType() const;
    void updatePlayer();
    void updateCollision();
    void updateGui();
    void update();
    void renderGui(sf::RenderTarget* target);
    void render();
};