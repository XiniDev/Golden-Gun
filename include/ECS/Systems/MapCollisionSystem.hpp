#pragma once

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ECS/System.hpp"

#include "ECS/Components/BoxCollider.hpp"
// #include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"

struct MapCollisionSystem : public System
{
    // constructors & destructors
    MapCollisionSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& boxCollider = p_ecs.getComponent<BoxCollider>(entity);
            auto& rigidBody = p_ecs.getComponent<RigidBody>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            for (auto const& other: boxCollider.collidingEntities)
            {
                // ensures that the entity isn't itself and the other entity is a static map object
                if (entity == other || !p_ecs.getComponent<BoxCollider>(other).isStatic) continue;

                SDL_Rect intersect;
                auto const& otherBoxCollider = p_ecs.getComponent<BoxCollider>(other);

                // on intersection, push the entity back depending on the depth of intersection
                if (SDL_IntersectRect(&boxCollider.box, &otherBoxCollider.box, &intersect))
                {
                    // determines which axis has a smaller intersection and bounces the entity on that axis backwards
                    if (intersect.h < intersect.w)
                        transform.position.y += (boxCollider.box.y > otherBoxCollider.box.y) ? static_cast<int>(intersect.h) : static_cast<int>(-intersect.h);
                    else
                        transform.position.x += (boxCollider.box.x > otherBoxCollider.box.x) ? static_cast<int>(intersect.w) : static_cast<int>(-intersect.w);

                    // hitting ceiling causes velocity y to become 0
                    if (intersect.h < intersect.w && boxCollider.box.y > otherBoxCollider.box.y) rigidBody.velocity.y = 0;

                    // update the box collider's position based on new position
                    boxCollider.box.x = static_cast<int>((transform.position.x + boxCollider.offset.x * p_gc.scale) * transform.scale.x);
                    boxCollider.box.y = static_cast<int>((transform.position.y + boxCollider.offset.y * p_gc.scale) * transform.scale.y);

                    // update the groundCheck position based on box collider's position
                    if (!SDL_RectEmpty(&boxCollider.groundCheck))
                    {
                        boxCollider.groundCheck.x = static_cast<int>(boxCollider.box.x);
                        boxCollider.groundCheck.y = static_cast<int>(boxCollider.box.y + boxCollider.box.h);
                    }
                }
            }
        }
    }
};