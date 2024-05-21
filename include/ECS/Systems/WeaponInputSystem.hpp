#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "ECS/System.hpp"

#include "ECS/Components/WeaponProperties.hpp"

// #include "ECS/Components/ControllableComponent.hpp"

struct WeaponInputSystem : public System
{
    // constructors & destructors
    WeaponInputSystem()
    {}

    // logic
    
    void handleEvents(ECSManager& p_ecs, SDL_Event* p_event, const Uint8* p_keys) override
    {
        for (auto const& entity: entities)
        {
            auto& weaponProperties = p_ecs.getComponent<WeaponProperties>(entity);

            // handle mouse button press
            if (p_event->type == SDL_MOUSEBUTTONDOWN && p_event->button.button == SDL_BUTTON_LEFT)
            {
                weaponProperties.isAttacking = true;
            }
            // handle mouse button release
            else if (p_event->type == SDL_MOUSEBUTTONUP && p_event->button.button == SDL_BUTTON_LEFT)
            {
                weaponProperties.isAttacking = false;
            }
        }
    }
};