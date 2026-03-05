#include "menu.h"

Menu::Menu() {
    window = new sf::RenderWindow();
    winclose = new sf::RectangleShape();
    font = new sf::Font();
    image = new sf::Texture();
    bg = new sf::Sprite();

    set_values();
}

Menu::~Menu() {
    delete window;
    delete winclose;
    delete font;
    delete image;
    delete bg;
}

void Menu::set_values() {
    // Get desktop mode for fullscreen
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    window->create(desktop, "Menu SFML", sf::Style::Fullscreen);

    pos = 0;
    pressed = theselect = false;
    font->loadFromFile("Assets/Fonts/PixellettersFull.ttf");
    image->loadFromFile("Assets/Textures/Background/mainmenu.png");

    // Scale background to fill screen
    bg->setTexture(*image);
    float scaleX = static_cast<float>(desktop.width) / image->getSize().x;
    float scaleY = static_cast<float>(desktop.height) / image->getSize().y;
    bg->setScale(scaleX, scaleY);

    pos_mouse = { 0,0 };
    mouse_coord = { 0, 0 };

    options = { "Play", "Quit" };
    texts.resize(2); // Reduced to 2 options

    // Adjust coordinates for fullscreen
    coords = {
        {desktop.width / 2.f - 50, desktop.height / 2.f - 50},
        {desktop.width / 2.f - 50, desktop.height / 2.f + 50}
    };

    int baseFontSize = 80;  // Increased base font size
    sizes = { baseFontSize, baseFontSize };

    for (std::size_t i{}; i < texts.size(); ++i) {
        texts[i].setFont(*font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(sizes[i]);
        texts[i].setFillColor(sf::Color::White);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setOutlineThickness(2);
        texts[i].setPosition(coords[i]);
    }
    texts[0].setOutlineThickness(4); // Highlight first option
    pos = 0;

    // Optional: Close button (adjust as needed)
    winclose->setSize(sf::Vector2f(50, 50));
    winclose->setPosition(desktop.width - 60, 10);
    winclose->setFillColor(sf::Color::Transparent);
}

void Menu::loop_events() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
            return;
        }

        pos_mouse = sf::Mouse::getPosition(*window);
        mouse_coord = window->mapPixelToCoords(pos_mouse);

        // Handle keyboard navigation
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !pressed) {
            if (pos < 1) {
                ++pos;
                texts[pos].setOutlineThickness(4);
                texts[pos - 1].setOutlineThickness(2);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !pressed) {
            if (pos > 0) {
                --pos;
                texts[pos].setOutlineThickness(4);
                texts[pos + 1].setOutlineThickness(2);
            }
        }

        // Handle Enter key
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            if (pos == 0) { // Play selected
                window->close();
                return;
            }
            if (pos == 1) { // Quit selected
                window->close();
                exit(0);
            }
        }

        // Handle mouse click on close or menu options
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (winclose->getGlobalBounds().contains(mouse_coord)) {
                window->close();
                exit(0);
            }

            // Check if mouse clicks on menu options
            for (size_t i = 0; i < texts.size(); ++i) {
                if (texts[i].getGlobalBounds().contains(mouse_coord)) {
                    if (i == 0) { // Play
                        window->close();
                        return;
                    }
                    if (i == 1) { // Quit
                        window->close();
                        exit(0);
                    }
                }
            }
        }
    }
}

void Menu::draw_all() {
    window->clear();
    window->draw(*bg);
    for (auto& t : texts) {
        window->draw(t);
    }
    window->display();
}

bool Menu::run_menu() {
    while (window->isOpen()) {
        loop_events();
        draw_all();
    }
    return true;
}