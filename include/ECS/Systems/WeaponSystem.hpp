#pragma once

#include <cmath>

#include "Vector2f.hpp"
#include "EntityFactory.hpp"
#include "Utils.hpp"

#include "ECS/System.hpp"

#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/WeaponProperties.hpp"

struct WeaponSystem : public System
{
    // constructors & destructors
    WeaponSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& transform = p_ecs.getComponent<Transform>(entity);
            auto& weaponProperties = p_ecs.getComponent<WeaponProperties>(entity);

            if (weaponProperties.attackSpeedCD >= 1 / weaponProperties.attackSpeed)
            {
                if (weaponProperties.isAttacking)
                {
                    // reset attack speed cooldown to start delaying attack again
                    weaponProperties.attackSpeedCD = 0;

                    // trigger shooting animation
                    auto& weaponAnimation = p_ecs.getComponent<Animation>(entity);
                    weaponAnimation.name = AnimationName::SHOOT;

                    // change frame ratio multiplier based on attack speed
                    weaponAnimation.frameRatioMultiplier = weaponProperties.attackSpeed / 2;

                    // reset frame counter to start animation from the beginning
                    weaponAnimation.frameCounter = 0;

                    // convert to radians, use negative because rotation is inverse
                    const float rotationRadians = -transform.rotation * (M_PI / 180.0f);

                    // calculate thetas based on rotation
                    const float cosTheta = std::cos(rotationRadians);
                    const float sinTheta = std::sin(rotationRadians);

                    // calculate offset for the bullet position so it can be applied to transform before instantiation
                    Vector2 offset = weaponProperties.bulletPositionOffset;

                    // ensure offset is the same as the gun flip
                    const auto& sprite = p_ecs.getComponent<Sprite>(entity);
                    if (sprite.flip == SDL_RendererFlip::SDL_FLIP_VERTICAL)
                        offset.y -= sprite.src.h / 2;

                    // offset rotation
                    const Vector2 rotatedOffset = utils::applyRotationMatrix(offset, transform.rotation);

                    // offset position
                    const Vector2 bulletPositionOffset = Vector2(rotatedOffset.x * p_gc.scale * transform.scale.x,
                                                                 rotatedOffset.y * p_gc.scale * transform.scale.y);
                    const Vector2 bulletPosition = transform.position + bulletPositionOffset;

                    // package into transform to put as an initial transform for the bullet
                    const Transform bulletTransform = Transform
                    {
                        .position = bulletPosition,
                        .rotation = transform.rotation,
                        .rotationPoint = RotationPoint::WEAPON,
                        .scale = Vector2f(1.0f, 1.0f)
                    };

                    // instantiate bullet
                    Entity bullet = entityFactory::makeBullet(p_ecs, p_gc, p_rm, bulletTransform);

                    // make sure bullet has same flip as gun
                    auto& bulletSprite = p_ecs.getComponent<Sprite>(bullet);
                    if (bulletTransform.rotation > 90.0f || bulletTransform.rotation < -90.0f)
                        bulletSprite.flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
                    else
                        bulletSprite.flip = SDL_RendererFlip::SDL_FLIP_NONE;

                    auto& bulletRigidBody = p_ecs.getComponent<RigidBody>(bullet);

                    // for bullet speed
                    const float bulletSpeed = weaponProperties.bulletSpeed;

                    // change bullet acceleration based on direction and speed
                    // for direction, x is cosine for a/h; y is sine for o/h
                    bulletRigidBody.acceleration.x = cosTheta * bulletSpeed;
                    bulletRigidBody.acceleration.y = sinTheta * bulletSpeed;

                    // std::cout << transform.rotation << std::endl;
                }
            }
            else
            {
                // increment for attack speed cooldown
                weaponProperties.attackSpeedCD += (weaponProperties.attackSpeedCD < 1 / weaponProperties.attackSpeed) ? p_dt : 0;
            }
        }
    }
};