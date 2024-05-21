#pragma once

#include <iostream>
#include <SDL2/SDL.h>

#include "ECS/System.hpp"

#include "ECS/Components/MoveProperties.hpp"
// #include "ECS/Components/ControllableComponent.hpp"

struct MovementInputSystem : public System
{
    // constructors & destructors
    MovementInputSystem()
    {}

    // logic
    
    void handleEvents(ECSManager& p_ecs, SDL_Event* p_event, const Uint8* p_keys) override
    {
        for (auto const& entity: entities)
        {
            // auto const& playerProperties = p_ecs.getComponent<ControllableComponent>(entity);
            auto& moveProperties = p_ecs.getComponent<MoveProperties>(entity);

            // mapping keyboard input to entity properties
            moveProperties.moveDirection = p_keys[SDL_SCANCODE_D] - p_keys[SDL_SCANCODE_A];
            moveProperties.isSprinting = p_keys[SDL_SCANCODE_LSHIFT];

            // jump when pressing space
            if (moveProperties.jumpStage == JumpStage::NO_JUMP &&
                (moveProperties.isGrounded || moveProperties.lastOnGround < 15) &&
                p_keys[SDL_SCANCODE_SPACE])
                moveProperties.jumpStage = JumpStage::BEGIN_JUMP;
        }
    }
};