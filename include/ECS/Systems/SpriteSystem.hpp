#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ECS/System.hpp"

#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Transform.hpp"

struct SpriteSystem : public System
{
    // constructors & destructors
    SpriteSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& sprite = p_ecs.getComponent<Sprite>(entity);
            auto const& transform = p_ecs.getComponent<Transform>(entity);

            sprite.src = SDL_Rect
            {
                sprite.offset.x,
                sprite.offset.y,
                sprite.src.w,
                sprite.src.h
            };

            sprite.dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(sprite.src.w * transform.scale.x * p_gc.scale),
                static_cast<int>(sprite.src.h * transform.scale.y * p_gc.scale)
            };
        }
    }
    
    void render(ECSManager& p_ecs, SDL_Renderer* p_renderer, const float p_alpha) override
    {
        for (auto const& entity: entities)
        {
            auto const& sprite = p_ecs.getComponent<Sprite>(entity);
            auto const& transform = p_ecs.getComponent<Transform>(entity);

            SDL_Point rotationPoint;

            switch (transform.rotationPoint)
            {
                case RotationPoint::CENTER:
                {
                    rotationPoint = {sprite.dst.w / 2, sprite.dst.h / 2};
                    break;
                }

                case RotationPoint::ZERO:
                {
                    rotationPoint = {0, 0};
                    break;
                }

                case RotationPoint::WEAPON:
                {
                    const int pointX = 0;
                    const int pointY = sprite.dst.h / 2;
                    rotationPoint = {pointX, pointY};
                    break;
                }

                default:
                {
                    rotationPoint = {sprite.dst.w / 2, sprite.dst.h / 2};
                    break;
                }
            }

            if (sprite.texture != nullptr)
            {
                SDL_RenderCopyEx(p_renderer, sprite.texture, &sprite.src, &sprite.dst, transform.rotation, &rotationPoint, sprite.flip);
            }
            else
            {
                std::cout << "Entity with id: " << entity << " has a missing texture." << std::endl;
                continue;
            }
        }
    }
};