#pragma once

#include <SDL2/SDL.h>

#include "Vector2.hpp"

struct Sprite
{
    Vector2 offset;
    SDL_Rect src;
    SDL_Rect dst;
    SDL_Texture* texture;
    SDL_RendererFlip flip;
};