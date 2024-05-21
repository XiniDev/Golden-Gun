#pragma once

#include <cmath>

#include "Vector2f.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"

struct PhysicsSystem : public System
{
    // constructors & destructors
    PhysicsSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& rigidBody = p_ecs.getComponent<RigidBody>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            // Start X integration
            // X axis is integrated first
            // semi-implicit Euler by integrating velocity first then position
            rigidBody.velocity.x += rigidBody.acceleration.x * p_gc.scale * p_dt;
            rigidBody.velocity.x -= rigidBody.gravity.x * p_gc.scale * p_dt;

            // applying friction
            rigidBody.velocity.x *= 1 / (1 + (rigidBody.friction * p_dt));

            // round the velocity for pixel perfect movement
            // pixel perfect movement is scaled based on game config scale, so that the movement doesn't look jittery
            // each "pixel" = p_gc.scale amount of real pixels
            const float velX = rigidBody.moveRemainder.x + rigidBody.velocity.x * p_dt;
            const int roundedVelX = static_cast<int>(velX);
            rigidBody.moveRemainder.x = static_cast<float>(std::fmod(velX, roundedVelX));
            if (std::isnan(rigidBody.moveRemainder.x)) rigidBody.moveRemainder.x = velX;

            // position of X incremented by rounded velocity
            transform.position.x += roundedVelX;

            // Start Y integration
            // Y axis is integrated after X
            // semi-implicit Euler for y
            rigidBody.velocity.y += rigidBody.acceleration.y * p_gc.scale * p_dt;
            rigidBody.velocity.y -= rigidBody.gravity.y * p_gc.scale * p_dt;

            // ensures that there is a max fall speed
            rigidBody.velocity.y = static_cast<float>(std::fmax(rigidBody.velocity.y, -rigidBody.maxVelocity.y * p_gc.scale));

            // pixel perfect movement for y
            const float velY = rigidBody.moveRemainder.y + rigidBody.velocity.y * p_dt;
            const int roundedVelY = static_cast<int>(velY);
            rigidBody.moveRemainder.y = static_cast<float>(std::fmod(velY, roundedVelY));
            if (std::isnan(rigidBody.moveRemainder.y)) rigidBody.moveRemainder.y = velY;

            // position for y is inverted because entities are rendred from the top of the screen
            transform.position.y -= roundedVelY;
            // end
        }
    }
};