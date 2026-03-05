#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Menu {
private:
    sf::RenderWindow* window;
    sf::RectangleShape* winclose;
    sf::Font* font;
    sf::Texture* image;
    sf::Sprite* bg;

    int pos;
    bool pressed, theselect;
    sf::Vector2i pos_mouse;
    sf::Vector2f mouse_coord;

    std::vector<std::string> options;
    std::vector<sf::Text> texts;
    std::vector<sf::Vector2f> coords;
    std::vector<int> sizes;

public:
    Menu();
    ~Menu();

    void set_values();
    void loop_events();
    void draw_all();
    bool run_menu(); // Changed to return a bool to indicate game start
};