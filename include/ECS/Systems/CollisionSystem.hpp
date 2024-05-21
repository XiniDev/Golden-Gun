#pragma once

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Utils.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/Transform.hpp"

struct CollisionSystem : public System
{
    // constructors & destructors
    CollisionSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& boxCollider = p_ecs.getComponent<BoxCollider>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            // update the box collider's position based on position
            // boxCollider.box.x = static_cast<int>((transform.position.x + boxCollider.offset.x * p_gc.scale) * transform.scale.x);
            // boxCollider.box.y = static_cast<int>((transform.position.y + boxCollider.offset.y * p_gc.scale) * transform.scale.y);

            const Vector2 rotatedOffset = utils::applyRotationMatrix(Vector2(boxCollider.offset.x, boxCollider.offset.y), transform.rotation);
            boxCollider.box.x = static_cast<int>((transform.position.x + rotatedOffset.x * p_gc.scale) * transform.scale.x);
            boxCollider.box.y = static_cast<int>((transform.position.y + rotatedOffset.y * p_gc.scale) * transform.scale.y);

            // update the groundCheck position based on box collider's position
            if (!SDL_RectEmpty(&boxCollider.groundCheck))
            {
                boxCollider.groundCheck.x = static_cast<int>(boxCollider.box.x);
                boxCollider.groundCheck.y = static_cast<int>(boxCollider.box.y + boxCollider.box.h);
            }

            // reset collidingEntities and collidingGrounds vectors
            boxCollider.collidingEntities.clear();
            boxCollider.collidingGrounds.clear();

            if (!boxCollider.isStatic)
            {
                for (auto const& other: entities)
                {
                    // ensures that the collision check isn't against itself
                    if (entity == other) continue;

                    // store all box collisions by the box collider Rect in the collidingEntities vector
                    auto const& otherBoxCollider = p_ecs.getComponent<BoxCollider>(other);
                    if (SDL_HasIntersection(&boxCollider.box, &otherBoxCollider.box))
                        boxCollider.collidingEntities.push_back(other);

                    // store all ground intersections by the ground check Rect in the collidingGrounds vector
                    SDL_Rect groundIntersection;
                    if (SDL_IntersectRect(&boxCollider.groundCheck, &otherBoxCollider.box, &groundIntersection))
                    {
                        if (groundIntersection.h == boxCollider.groundCheck.h)
                            boxCollider.collidingGrounds.push_back(other);
                    }
                }
            }
        }
    }

    void render(ECSManager& p_ecs, SDL_Renderer* p_renderer, const float p_alpha) override
    {
        for (auto const& entity: entities)
        {
            auto const& boxCollider = p_ecs.getComponent<BoxCollider>(entity);

            if (boxCollider.debug)
            {
                SDL_SetRenderDrawColor(p_renderer, 0, 0, 255, 255);

                // draw box collider
                SDL_RenderDrawRect(p_renderer, &boxCollider.box);

                // if ground check is enabled for the entity, then draw it
                if (!SDL_RectEmpty(&boxCollider.groundCheck))
                    SDL_RenderDrawRect(p_renderer, &boxCollider.groundCheck);
            }
        }
    }
};