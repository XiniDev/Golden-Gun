#pragma once

#include "Vector2.hpp"

struct WeaponProperties
{
    bool isAttacking;
    float attackSpeed;
    Vector2 bulletPositionOffset;
    float bulletSpeed;

    // empty on init
    float attackSpeedCD;
};