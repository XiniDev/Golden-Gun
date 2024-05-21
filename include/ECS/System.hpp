#pragma once

#include <iostream>//debug
#include <set>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "GameConfig.hpp"
#include "Types.hpp"

class ECSManager;

class System
{
    public:
        std::set<Entity> entities;

        // all systems will have a default update function
        virtual void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) {};

        // all systems will have a default handle events function
        virtual void handleEvents(ECSManager& p_ecs, SDL_Event* p_event, const Uint8* p_keys) {};

        // all systems will have a default render function
        virtual void render(ECSManager& p_ecs, SDL_Renderer* p_renderer, const float p_alpha) {};
};