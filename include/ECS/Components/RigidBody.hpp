#pragma once

#include "Vector2f.hpp"

struct RigidBody
{
    Vector2f velocity;
    Vector2f acceleration;
    Vector2f gravity;
    float friction;

    Vector2f maxVelocity;
    Vector2f moveRemainder;
};