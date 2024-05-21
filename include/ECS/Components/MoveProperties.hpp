#pragma once

#include "Vector2f.hpp"

// jump stage allows cross system interaction rather than just using a generic isJumping bool
// systems can detect if entities need to start jumping or is in the middle of a jump
enum JumpStage
{
    NO_JUMP,
    BEGIN_JUMP,
    JUMPING,
    FALLING,
};

struct MoveProperties
{
    int moveDirection;
    JumpStage jumpStage;
    bool isGrounded;
    bool isSprinting;
    float sprintModifier;
    float frictionModifier;
    float moveAccel;
    float moveDecel;
    Vector2f gravity;
    float airAccelRate;
    float jumpForce;
    float jumpHangTimeThreshold;
    float jumpHangMaxSpeed;
    float jumpHangAccelerationMult;

    // empty on init
    int lastOnGround;
};