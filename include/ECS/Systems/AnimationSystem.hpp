#pragma once

#include <cstdint>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "ECS/System.hpp"

#include "ECS/Components/Animation.hpp"
#include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Sprite.hpp"

struct AnimationSystem : public System
{
    // constructors & destructors
    AnimationSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto& animation = p_ecs.getComponent<Animation>(entity);
            auto& sprite = p_ecs.getComponent<Sprite>(entity);

            AnimationName animationName = animationTypeChange(p_ecs, entity, animation.name);
            sprite.flip = checkFlip(p_ecs, entity, sprite.flip);

            if (const auto search = animation.infoMap.find(animationName); search != std::end(animation.infoMap))
            {
                animation.name = animationName;
                const AnimationInfo& animationInfo = search->second;
                const float frameRatio = p_gc.fps / (12 * animation.frameRatioMultiplier);

                sprite.src = SDL_Rect
                {
                    static_cast<int>(sprite.offset.x + static_cast<int>(animation.frameCounter / frameRatio) * animationInfo.frameOffset.x),
                    static_cast<int>(sprite.offset.y + animationInfo.rowNumber * animationInfo.frameOffset.y),
                    static_cast<int>(sprite.src.w),
                    static_cast<int>(sprite.src.h)
                };

                animation.frameCounter = frameCounterChange(p_ecs, p_gc, p_dt, entity, animationName, animationInfo, frameRatio, animation.frameCounter);
            }
        }
    }

    AnimationName animationTypeChange(ECSManager& p_ecs, Entity p_entity, const AnimationName& p_originalAnimationName)
    {
        AnimationName animationName = p_originalAnimationName;

        try
        {
            auto const& moveProperties = p_ecs.getComponent<MoveProperties>(p_entity);

            if (moveProperties.jumpStage != JumpStage::NO_JUMP)
            {
                if (moveProperties.moveDirection != 0)
                    animationName = AnimationName::SPRINT_JUMP;
                else
                    animationName = AnimationName::JUMP;
            }
            else
            {
                if (moveProperties.moveDirection != 0)
                    animationName = AnimationName::SPRINT;
                else
                    animationName = AnimationName::IDLE;
            }
        }
        catch (const std::runtime_error& e)
        {
            animationName = p_originalAnimationName;
        }

        return animationName;
    }

    SDL_RendererFlip checkFlip(ECSManager& p_ecs, Entity p_entity, const SDL_RendererFlip& p_originalFlip)
    {
        SDL_RendererFlip flip = p_originalFlip;

        try
        {
            auto const& moveProperties = p_ecs.getComponent<MoveProperties>(p_entity);

            if (moveProperties.moveDirection < 0)
                flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
            else if (moveProperties.moveDirection > 0)
                flip = SDL_RendererFlip::SDL_FLIP_NONE;
        }
        catch (const std::runtime_error& e)
        {
            flip = p_originalFlip;
        }

        return flip;
    }

    std::uint16_t frameCounterChange(ECSManager& p_ecs, const GameConfig& p_gc, float p_dt, Entity p_entity, const AnimationName& p_animationName, const AnimationInfo& p_animationInfo, const float p_frameRatio, const std::uint16_t p_currentFrame)
    {
        std::uint16_t frameCounter = p_currentFrame;
        std::uint16_t maxFrameCount = static_cast<std::uint16_t>((p_animationInfo.totalFrames * p_frameRatio));
        std::uint16_t newCount = (frameCounter + 1) % maxFrameCount;

        if (p_animationInfo.type == AnimationType::VELOCITY_MAP)
        {
            if (p_animationName == AnimationName::SPRINT_JUMP || p_animationName == AnimationName::JUMP)
            {
                try
                {
                    auto const& moveProperties = p_ecs.getComponent<MoveProperties>(p_entity);
                    auto const& rigidBody = p_ecs.getComponent<RigidBody>(p_entity);

                    frameCounter = mapVelocityToFrameJump(rigidBody.velocity.y, -rigidBody.maxVelocity.y * p_gc.scale, moveProperties.jumpForce * p_gc.scale * p_dt, p_animationInfo.totalFrames * p_frameRatio - 1);
                }
                catch (const std::runtime_error& e)
                {
                    frameCounter = newCount;
                }
            }
        }
        else if (p_animationInfo.type == AnimationType::ONE_TIME)
        {
            if (p_animationName == AnimationName::SHOOT)
            {
                if (frameCounter < maxFrameCount - 1)
                    frameCounter++;
            }
            else if (p_animationName == AnimationName::BULLET)
            {
                if (frameCounter < maxFrameCount - 1)
                    frameCounter++;
            }
        }
        else
            // increment the animation frame counter or loop if at the last frame
            frameCounter = newCount;
        

        return frameCounter;
    }

    std::uint16_t mapVelocityToFrameJump(float velY, float maxVelY, float jumpForce, std::uint16_t totalFrames)
    {
        // normalize y velocity to a ratio between 0-1
        float normalized = (velY - maxVelY) / (jumpForce - maxVelY);

        // scale the normalized ratio to the total frame count (inverted for increment based on decrement of velocity)
        float frameCount = totalFrames * (1 - normalized);

        // debug
        // std::cout << "vely: " << velY << " | total frames: " << totalFrames << " | count: " << frameCount << " | casted count: " << static_cast<std::uint16_t>(frameCount) << std::endl;

        return static_cast<std::uint16_t>(frameCount);
    }
};