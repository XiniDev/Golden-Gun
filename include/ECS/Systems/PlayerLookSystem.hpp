#pragma once

#include "Utils.hpp"
#include "Vector2f.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Sprite.hpp"

// #include "ECS/Components/ControllableComponent.hpp"

struct PlayerLookSystem : public System
{
    // constructors & destructors
    PlayerLookSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            // controls sprite look for the player by changing the sprite flip based on player look
            auto& sprite = p_ecs.getComponent<Sprite>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            Vector2 rotationPoint = {transform.position.x + sprite.dst.w / 2, transform.position.y + sprite.dst.h / 2};

            // if rotation 
            float rotation = utils::getRotationAngle(p_gc.mouseState, rotationPoint);
            if (rotation > 90.0f || rotation < -90.0f)
                sprite.flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
            else
                sprite.flip = SDL_RendererFlip::SDL_FLIP_NONE;
        }
    }
};