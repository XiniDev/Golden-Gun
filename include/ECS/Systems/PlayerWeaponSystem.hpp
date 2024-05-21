#pragma once

#include "Utils.hpp"
#include "Vector2f.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Sprite.hpp"

// #include "ECS/Components/ControllableComponent.hpp"

struct PlayerWeaponSystem : public System
{
    // constructors & destructors
    PlayerWeaponSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            // controls weapon look for the player by changing the weapon rotation based on player look
            auto& entityLink = p_ecs.getComponent<EntityLink>(entity);
            auto& sprite = p_ecs.getComponent<Sprite>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            auto const& linkedSprite = p_ecs.getComponent<Sprite>(entityLink.mainEntity);
            auto const& linkedTransform = p_ecs.getComponent<Transform>(entityLink.mainEntity);

            Vector2 rotationPoint = {linkedTransform.position.x + linkedSprite.dst.w / 2, linkedTransform.position.y + linkedSprite.dst.h / 2};

            transform.rotation = utils::getRotationAngle(p_gc.mouseState, rotationPoint);
            if (transform.rotation > 90.0f || transform.rotation < -90.0f)
                sprite.flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
            else
                sprite.flip = SDL_RendererFlip::SDL_FLIP_NONE;
        }
    }
};