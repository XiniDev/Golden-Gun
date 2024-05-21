#pragma once

#include "ECS/Types.hpp"
#include "Vector2.hpp"

struct EntityLink
{
    Entity mainEntity;
    Vector2 posOffset;
};