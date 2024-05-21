#pragma once

#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ECS/System.hpp"

#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"

struct JumpMapCollisionSystem : public System
{
    // constructors & destructors
    JumpMapCollisionSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& boxCollider = p_ecs.getComponent<BoxCollider>(entity);
            auto& moveProperties = p_ecs.getComponent<MoveProperties>(entity);
            auto& rigidBody = p_ecs.getComponent<RigidBody>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            bool groundedCheck = false;

            for (auto const& other: boxCollider.collidingGrounds)
            {
                // ensures that the entity isn't itself and the other entity is a static map object
                if (entity == other || !p_ecs.getComponent<BoxCollider>(other).isStatic) continue;

                auto const& otherBoxCollider = p_ecs.getComponent<BoxCollider>(other);

                // on ground checks are based on the ground entities that has been collided to prevent extra checks
                // the entity must not be in jumping jump stage, this is to prevent this weird interaction where the entity slides on the corners for higher ledges
                if (moveProperties.jumpStage != JumpStage::JUMPING &&
                    boxCollider.box.y + boxCollider.box.h == otherBoxCollider.box.y &&
                    boxCollider.box.x + boxCollider.box.w > otherBoxCollider.box.x &&
                    boxCollider.box.x < otherBoxCollider.box.x + otherBoxCollider.box.w)
                {
                    moveProperties.isGrounded = true;
                    groundedCheck = true;
                }
            }

            if (!groundedCheck)
                moveProperties.isGrounded = false;
        }
    }
};