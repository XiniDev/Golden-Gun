#pragma once

#include "Vector2.hpp"
#include "Vector2f.hpp"

enum RotationPoint
{
    CENTER,
    ZERO,
    WEAPON,
};

struct Transform
{
    Vector2 position;
    float rotation;
    RotationPoint rotationPoint;
    Vector2f scale;
};