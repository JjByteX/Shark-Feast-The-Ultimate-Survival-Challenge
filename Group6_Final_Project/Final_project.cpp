#include "Game.h"
#include "menu.h"

int main()
{
    // Initialize random seed
    srand(static_cast<unsigned>(time(0)));

    // Create and run the menu first
    Menu* menu = new Menu();
    menu->run_menu();
    delete menu;
    menu = nullptr;

    // After menu closes, start the game
    Game game;

    // Game loop
    while (game.running())
    {
        game.update();
        game.render();
    }

    // End of application
    return 0;
}