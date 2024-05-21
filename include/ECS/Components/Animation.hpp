#pragma once

#include <cstdint>
#include <unordered_map>
#include <utility>

#include <SDL2/SDL.h>

#include "Vector2f.hpp"

enum AnimationName
{
    IDLE,
    IDLE_TRANSITION,
    SPRINT,
    SPRINT_JUMP,
    JUMP,
    SHOOT,
    BULLET,
};

enum AnimationType
{
    CONTINUOUS,
    ONE_TIME,
    VELOCITY_MAP
};

struct AnimationInfo {
    std::uint16_t rowNumber;
    std::uint16_t totalFrames;
    Vector2f frameOffset;
    AnimationType type;
};

struct Animation
{
    // animation type -> animation info
    std::unordered_map<AnimationName, AnimationInfo> infoMap;

    AnimationName name;
    std::uint16_t frameCounter;
    float frameRatioMultiplier;
};