#pragma once

#include <vector>

#include <SDL2/SDL.h>

#include "ECS/Types.hpp"
#include "Vector2.hpp"

struct BoxCollider
{
    SDL_Rect box;
    Vector2 offset;
    bool isStatic;
    SDL_Rect groundCheck;
    bool debug;

    // empty on init
    std::vector<Entity> collidingEntities;
    std::vector<Entity> collidingGrounds;
};