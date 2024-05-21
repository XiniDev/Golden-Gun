#pragma once

#include <cmath>

#include "Vector2f.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"

struct MovementSystem : public System
{
    // constructors & destructors
    MovementSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& moveProperties = p_ecs.getComponent<MoveProperties>(entity);
            auto& rigidBody = p_ecs.getComponent<RigidBody>(entity);
            // auto& transform = p_ecs.getComponent<Transform>(entity);

            // Start movement calculation for X

            // rigidBody gravity is set to a custom value in moveProperties (different from other entities)
            rigidBody.gravity.x = moveProperties.gravity.x;

            // calculate velocity modifiers
            const float velModX = (moveProperties.isSprinting) ? moveProperties.sprintModifier : 1;

            // find the target velocity that the entity needs to accelerate or decelerate towards
            float targetVelX = moveProperties.moveDirection * rigidBody.maxVelocity.x * velModX * p_gc.scale;
            targetVelX = std::lerp(rigidBody.velocity.x, targetVelX, 0.6f);

            // set acceleration rate based on the need to accelerate or decelerate
            float accelRateX = (std::abs(targetVelX) > 0.0f) ? moveProperties.moveAccel : moveProperties.moveDecel;

            // reduce acceleration rate when air borne
            if (!moveProperties.isGrounded)
                accelRateX *= moveProperties.airAccelRate;

            // when entity is jumping at its peak, increase target velocity and acceleration rate
            if (moveProperties.jumpStage != JumpStage::NO_JUMP && std::abs(rigidBody.velocity.y) < moveProperties.jumpHangTimeThreshold)
            {
                targetVelX *= moveProperties.jumpHangMaxSpeed;
                accelRateX *= moveProperties.jumpHangAccelerationMult;
            }

            // conserve momentum if current velocity is greater than max velocity
            if (std::abs(rigidBody.velocity.x) > std::abs(targetVelX) &&
                std::signbit(rigidBody.velocity.x) == std::signbit(targetVelX) &&
                std::abs(targetVelX) > 0.0f &&
                !moveProperties.isGrounded)
                accelRateX = 0.0f;

            // get the difference between the target velocity and the current velocity
            const float velDiffX = targetVelX - rigidBody.velocity.x;

            // set acceleration scaled based on velocity difference
            rigidBody.acceleration.x = velDiffX * accelRateX;

            // friction to slow down velocity when not moving quicker (for snappy stopping)
            rigidBody.friction = (moveProperties.moveDirection == 0 && moveProperties.isGrounded) ? moveProperties.frictionModifier : 0.0f;

            // Start movement calculation for Y
            rigidBody.acceleration.y = 0.0f;

            // rigidBody gravity is set to a custom value in moveProperties (different from other entities)
            rigidBody.gravity.y = moveProperties.gravity.y;

            // if jump stage is begin jump, then increment acceleration by the jump force
            if (moveProperties.jumpStage == JumpStage::BEGIN_JUMP)
            {
                moveProperties.jumpStage = JumpStage::JUMPING;
                moveProperties.isGrounded = false;
                rigidBody.acceleration.y += moveProperties.jumpForce;
                rigidBody.velocity.y = 0.0f;
            }
            // if jump stage is jumping
            else if (moveProperties.jumpStage == JumpStage::JUMPING)
            {
                // set jump state to falling if velocity of y is negative
                if (rigidBody.velocity.y < 0.0f) moveProperties.jumpStage = JumpStage::FALLING;

                // set the gravity weaker for hang time at peak of the jump
                if (std::abs(rigidBody.velocity.y) < moveProperties.jumpHangTimeThreshold)
                    rigidBody.gravity.y *= 0.5f;
            }
            // if jump stage is falling
            else if (moveProperties.jumpStage == JumpStage::FALLING)
            {
                // set the gravity weaker for hang time at peak of the jump
                if (std::abs(rigidBody.velocity.y) < moveProperties.jumpHangTimeThreshold)
                    rigidBody.gravity.y *= 0.5f;
                // double gravity on fall
                else
                    rigidBody.gravity.y *= 2.0f;
            }

            // if grounded, then reset the velocity so the object isn't in a constant falling state
            if (moveProperties.isGrounded)
            {
                // set jump stage back to no jump since entity is grounded
                moveProperties.jumpStage = JumpStage::NO_JUMP;
                rigidBody.velocity.y = 0.0f;
                rigidBody.gravity.y = 0.0f;

                // coyote time reset when grounded
                moveProperties.lastOnGround = 0;
            }
            // gravity is multiplied by scale as it is a movement factor
            else
            {
                // transition to falling stage
                if (moveProperties.jumpStage == JumpStage::NO_JUMP)
                    moveProperties.jumpStage = JumpStage::FALLING;

                // coyote time accumalates when not grounded
                moveProperties.lastOnGround++;
            }

            // debug
            // std::cout 
            //     << "jforce (u/d): " << moveProperties.jumpForce
            //     << " | ygrav (l/r): " << rigidBody.gravity.y
            //     << " | airacc (kp8/kp2): " << moveProperties.airAccelRate
            //     << " | jhtt (kp4/kp6): " << moveProperties.jumpHangTimeThreshold
            //     << " | vely: " << rigidBody.velocity.y
            //     << std::endl;
        }
    }
};