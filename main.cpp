#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Game.hpp"

int main(int argc, char* argv[])
{
    // SDL initialisation
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "Failed to initialise SDL2." << "\nError: " << SDL_GetError() << std::endl;

    if (IMG_Init(IMG_INIT_PNG) == 0)
        std::cout << "Failed to initialise SDL2_image." << "\nError: " << SDL_GetError() << std::endl;

    Game game;

    // load resources & setup
    game.loadResources();
    game.setUpECS();

    // game loop
    game.runGameLoop();

    // clean up
    game.cleanUp();

    return EXIT_SUCCESS;
}