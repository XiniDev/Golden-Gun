#pragma once

enum DamageType 
{
    BLUNT,
    PIERCE,
    SLASH,
    BLEED,
};

struct Damage
{
    float value;
    DamageType type;
};