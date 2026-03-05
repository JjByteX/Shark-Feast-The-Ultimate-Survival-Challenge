#include "Game.h"
#include <random>

void Game::initVariables()
{
    this->endGame = false;
    this->spawnTimerMax = 3.f;
    this->spawnTimer = this->spawnTimerMax;
    this->maxSwagBalls = 80;
    this->points = 0;
}

void Game::initBackground()
{
    // Load background texture
    if (!backgroundTexture.loadFromFile("Assets/Textures/Background/background.png"))
    {
        std::cout << "ERROR::GAME::INITBACKGROUND::COULD NOT LOAD BACKGROUND" << std::endl;
        return;
    }

    // Set the background sprite
    backgroundSprite.setTexture(backgroundTexture);

    // Optional: Scale the background to fit the window
    float scaleX = static_cast<float>(videoMode.width) / backgroundTexture.getSize().x;
    float scaleY = static_cast<float>(videoMode.height) / backgroundTexture.getSize().y;
    backgroundSprite.setScale(scaleX, scaleY);
}

void Game::initWindow()
{
    this->videoMode = sf::VideoMode(1920, 1080);
    this->window = new sf::RenderWindow(this->videoMode, "Shark Feast: The Ultimate Survival Challenge", sf::Style::Fullscreen | sf::Style::Titlebar);
    this->window->setFramerateLimit(60);

    // Initialize game view with more zoom
    this->gameView = this->window->getDefaultView();
    this->gameView.zoom(0.5f);  // Zoom in more aggressively
    this->cameraOffset = sf::Vector2f(0.f, 0.f);
}

sf::Vector2f Game::calculateCameraMovement()
{
    sf::Vector2f playerPos = this->player.getSprite().getPosition();
    sf::Vector2f viewSize = this->gameView.getSize();

    // Define tighter screen margins (30% of screen width/height)
    float marginX = viewSize.x * 0.3f;
    float marginY = viewSize.y * 0.3f;

    sf::Vector2f newOffset = this->cameraOffset;

    // Horizontal camera movement
    if (playerPos.x < this->cameraOffset.x + marginX)
    {
        newOffset.x = playerPos.x - marginX;
    }
    else if (playerPos.x > this->cameraOffset.x + viewSize.x - marginX)
    {
        newOffset.x = playerPos.x - (viewSize.x - marginX);
    }

    // Vertical camera movement
    if (playerPos.y < this->cameraOffset.y + marginY)
    {
        newOffset.y = playerPos.y - marginY;
    }
    else if (playerPos.y > this->cameraOffset.y + viewSize.y - marginY)
    {
        newOffset.y = playerPos.y - (viewSize.y - marginY);
    }

    return newOffset;
}

void Game::updateCamera()
{
    // Smoothly interpolate camera movement
    sf::Vector2f targetOffset = calculateCameraMovement();
    float smoothFactor = 0.1f;
    this->cameraOffset += (targetOffset - this->cameraOffset) * smoothFactor;

    // Get the size of the game view
    sf::Vector2f viewSize = this->gameView.getSize();

    // Calculate the boundaries for the camera
    float minX = 0.f; // Minimum X offset (left edge)
    float minY = 0.f; // Minimum Y offset (top edge)
    float maxX = backgroundSprite.getGlobalBounds().width - viewSize.x; // Maximum X offset (right edge)
    float maxY = backgroundSprite.getGlobalBounds().height - viewSize.y; // Maximum Y offset (bottom edge)

    // Clamp the camera offset to the boundaries
    if (this->cameraOffset.x < minX) {
        this->cameraOffset.x = minX;
    }
    else if (this->cameraOffset.x > maxX) {
        this->cameraOffset.x = maxX;
    }

    if (this->cameraOffset.y < minY) {
        this->cameraOffset.y = minY;
    }
    else if (this->cameraOffset.y > maxY) {
        this->cameraOffset.y = maxY;
    }

    // Update the game view
    this->gameView.setCenter(this->cameraOffset + this->gameView.getSize() / 2.f);
    this->window->setView(this->gameView);
}


void Game::initFonts()
{
    if (!this->font.loadFromFile("Assets/Fonts/PixellettersFull.ttf"))
    {
        std::cout << " ! ERROR::GAME::INITFONTS::COULD NOT LOAD PixellettersFull.ttf" << "\n";
    }
}

void Game::initText()
{
    //Gui text init
    this->guiText.setFont(this->font);
    this->guiText.setFillColor(sf::Color::White);
    this->guiText.setCharacterSize(32);

    //End game text
    this->endGameText.setFont(this->font);
    this->endGameText.setFillColor(sf::Color::Red);
    this->endGameText.setCharacterSize(60);
    this->endGameText.setPosition(sf::Vector2f(650, 400));
    this->endGameText.setString("YOU ARE DEAD!!! EXIT THE GAME!!!");
}

void Game::initMusic()
{
    if (!this->music.openFromFile("Assets/Sounds/BGM.mp3"))
    {
        std::cout << "ERROR::GAME::INITMUSIC::COULD NOT LOAD BGM.mp3" << "\n";
    }
    music.setLoop(true);
    music.play();
}

//Constructors and Destructors
Game::Game()
{
    // Seed the random number generator
    std::srand(std::time(nullptr));

    this->initVariables();
    this->initWindow();
    this->initBackground();
    this->initFonts();
    this->initText();
    this->initMusic();


}

Game::~Game()
{
    delete this->window;
}

const bool& Game::getEndGame() const
{
    return this->endGame;
}

//Functions
const bool Game::running() const
{
    return this->window->isOpen();
}

void Game::pollEvents()
{
    while (this->window->pollEvent(this->sfmlEvent))
    {
        switch (this->sfmlEvent.type)
        {
        case sf::Event::Closed:
            this->window->close();
            break;
        case sf::Event::KeyPressed:
            if (this->sfmlEvent.key.code == sf::Keyboard::Escape)
                this->window->close();
            break;
        default:
            break;
        }
    }
}

void Game::updateMousePosition()
{
    this->mousePos = sf::Mouse::getPosition(*this->window);
}

void Game::spawnSwagBalls()
{
    //Timer
    if (this->spawnTimer < this->spawnTimerMax)
        this->spawnTimer += 1.f;
    else
    {
        if (this->swagBalls.size() < this->maxSwagBalls)
        {
            this->swagBalls.push_back(SwagBall(*this->window, this->randBallType()));

            this->spawnTimer = 0.f;
        }
    }
}

const int Game::randBallType() const
{
    // Use more robust random number generation
    int type = SwagBallTypes::DEFAULT;

    int randValue = std::rand() % 100 + 1;

    // Adjust probabilities:
    // 60% chance for DEFAULT
    // 25% chance for HEALING
    // 15% chance for DAMAGING
    if (randValue <= 60)
        type = SwagBallTypes::DEFAULT;
    else if (randValue <= 85)
        type = SwagBallTypes::HEALING;
    else
        type = SwagBallTypes::DAMAGING;

    return type;
}

void Game::updatePlayer()
{
    this->player.update(this->window);

    if (this->player.getHp() <= 0)
        this->endGame = true;
}

void Game::updateCollision()
{
    //Check the collision
    for (size_t i = 0; i < this->swagBalls.size(); i++)
    {
        if (this->player.getSprite().getGlobalBounds().intersects(this->swagBalls[i].getShape().getGlobalBounds()))
        {
            switch (this->swagBalls[i].getType())
            {
            case SwagBallTypes::DEFAULT:
                this->points++;
                break;
            case SwagBallTypes::DAMAGING:
                this->player.takeDamage(2);
                break;
            case SwagBallTypes::HEALING:
                this->points++;
                this->player.gainHealth(1);
                break;
            }

            //Remove the ball
            this->swagBalls.erase(this->swagBalls.begin() + i);
            break; // Avoid iterator invalidation
        }
    }
}

void Game::updateGui()
{
    float elapsedSeconds = this->clock.getElapsedTime().asSeconds();

    std::stringstream ss;

    ss
        << "    WALLY " << "\t"
        << " POINTS: " << this->points << "\t"
        << " HEALTH: " << this->player.getHp() << " / " << this->player.getHpMax() << "\t"
        << " TIME: " << static_cast<int>(elapsedSeconds) << "\t";

    this->guiText.setString(ss.str());
    this->guiText.setOutlineColor(sf::Color::Black);
    this->guiText.setFillColor(sf::Color::Yellow);
    this->guiText.setOutlineThickness(2);
    this->guiText.setCharacterSize(60.f);
}

void Game::update()
{
    this->pollEvents();

    this->updateMousePosition();

    if (this->endGame == false)
    {
        this->spawnSwagBalls();
        this->updatePlayer();

        // Update and remove out-of-bounds balls
        for (auto it = swagBalls.begin(); it != swagBalls.end();)
        {
            it->update(*this->window);

            // Remove default and healing balls that reach the top of the screen
            if ((it->getType() == SwagBall::DEFAULT || it->getType() == SwagBall::HEALING) &&
                it->getShape().getPosition().y < 0)
            {
                it = swagBalls.erase(it);
            }
            else
            {
                ++it;
            }
        }

        this->updateCollision();
        this->updateGui();
        this->updateCamera();
    }
}

void Game::renderGui(sf::RenderTarget* target)
{
    target->draw(this->guiText);
}

void Game::render()
{
    this->window->clear();

    // Draw background first
    this->window->draw(backgroundSprite);

    // Render player
    this->player.render(this->window);

    // Render swag balls
    for (auto& ball : this->swagBalls)
    {
        ball.render(*this->window);
    }

    // Render gui (we'll need to reset view for UI)
    sf::View defaultView = this->window->getDefaultView();
    this->window->setView(defaultView);
    this->renderGui(this->window);

    // Restore game view
    this->window->setView(this->gameView);

    // Render end text
    if (this->endGame == true)
    {
        this->window->setView(defaultView);
        this->window->draw(this->endGameText);
    }

    this->window->display();
}