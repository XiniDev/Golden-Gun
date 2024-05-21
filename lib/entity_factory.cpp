#include "EntityFactory.hpp"

Entity entityFactory::makePlayer(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm)
{
    Entity player = p_ecs.createEntity();

    // add components

    p_ecs.addComponent(
        player,
        RigidBody
        {
            .velocity = Vector2f(),
            .acceleration = Vector2f(),
            .gravity = Vector2f(0.0f, 0.0f),
            .friction = 0.0f,
            .maxVelocity = Vector2f(50.0f, 400.0f),
            .moveRemainder = Vector2f()
        }
    );

    p_ecs.addComponent(
        player,
        Transform
        {
            .position = Vector2(static_cast<int>(50 * p_gc.scale), static_cast<int>(50 * p_gc.scale)),
            .rotation = 0.0f,
            .rotationPoint = RotationPoint::CENTER,
            .scale = Vector2f(1.0f, 1.0f)
        }
    );

    p_ecs.addComponent(
        player,
        MoveProperties
        {
            .moveDirection = 0,
            .jumpStage = JumpStage::NO_JUMP,
            .isGrounded = false,
            .isSprinting = false,
            .sprintModifier = 2.0f,
            .frictionModifier = 3.0f,
            .moveAccel = 8.0f,
            .moveDecel = 8.0f,
            .gravity = Vector2f(0.0f, 500.0f),
            .airAccelRate = 0.5f,
            .jumpForce = 25000.0f,
            .jumpHangTimeThreshold = 50.0f,
            .jumpHangMaxSpeed = 1.2f,
            .jumpHangAccelerationMult = 1.1f
        }
    );

    auto const& transform = p_ecs.getComponent<Transform>(player);
    SDL_Rect spriteSrc = SDL_Rect{0, 0, 50, 50};
    p_ecs.addComponent(
        player,
        Sprite
        {
            .offset = Vector2(spriteSrc.x, spriteSrc.y),
            .src = spriteSrc,
            .dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(spriteSrc.w * transform.scale.x * p_gc.scale),
                static_cast<int>(spriteSrc.h * transform.scale.y * p_gc.scale)
            },
            .texture = p_rm.getTextureCache().find("predator_sprite")->second
        }
    );

    Vector2f frameOffset = Vector2f(spriteSrc.w, spriteSrc.h);

    p_ecs.addComponent(
        player,
        Animation
        {
            .infoMap =
            {
                {AnimationName::IDLE, {0, 20, frameOffset, AnimationType::CONTINUOUS}},
                {AnimationName::IDLE_TRANSITION, {1, 2, frameOffset, AnimationType::ONE_TIME}},
                {AnimationName::SPRINT, {2, 6, frameOffset, AnimationType::CONTINUOUS}},
                {AnimationName::SPRINT_JUMP, {3, 7, frameOffset, AnimationType::VELOCITY_MAP}},
                {AnimationName::JUMP, {4, 7, frameOffset, AnimationType::VELOCITY_MAP}},
            },
            .name = AnimationName::IDLE,
            .frameCounter = 0,
            .frameRatioMultiplier = 1.0f
        }
    );

    Vector2 boxOffset = Vector2(19, 7);
    Vector2 boxSize = Vector2(12, 39);

    p_ecs.addComponent(
        player,
        BoxCollider
        {
            .box = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + boxOffset.y * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(boxSize.y * transform.scale.y * p_gc.scale)
            },
            .offset = boxOffset,
            .isStatic = false,
            .groundCheck = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + (boxOffset.y + boxSize.y) * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(1 * transform.scale.y * p_gc.scale)
            },
            .debug = true
        }
    );

    float maxHealth = 9.0f;

    p_ecs.addComponent(
        player,
        Health
        {
            .value = maxHealth,
            .max = maxHealth
        }
    );

    p_ecs.addComponent(
        player,
        ControllableComponent
        {}
    );

    return player;
}

Entity entityFactory::makePlayerWeapon(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, Entity p_linkedEntity)
{
    Entity playerWeapon = p_ecs.createEntity();

    // add components

    // player hold gun position
    Vector2 posOffset = Vector2(25, 17); // change this later - 25 = 19 + 12 / 2 (boxOffset + boxSize / 2 for x)

    p_ecs.addComponent(
        playerWeapon,
        EntityLink
        {
            .mainEntity = p_linkedEntity,
            .posOffset = posOffset
        }
    );

    auto const& linkedTransform = p_ecs.getComponent<Transform>(p_linkedEntity);

    p_ecs.addComponent(
        playerWeapon,
        Transform
        {
            .position = Vector2(static_cast<int>((linkedTransform.position.x + posOffset.x) * p_gc.scale),
                                static_cast<int>((linkedTransform.position.y + posOffset.y) * p_gc.scale)),
            .rotation = 0.0f,
            .rotationPoint = RotationPoint::WEAPON,
            .scale = Vector2f(1.0f, 1.0f)
        }
    );

    auto const& transform = p_ecs.getComponent<Transform>(playerWeapon);
    SDL_Rect spriteSrc = SDL_Rect{0, 0, 35, 13};
    p_ecs.addComponent(
        playerWeapon,
        Sprite
        {
            .offset = Vector2(spriteSrc.x, spriteSrc.y),
            .src = spriteSrc,
            .dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(spriteSrc.w * transform.scale.x * p_gc.scale),
                static_cast<int>(spriteSrc.h * transform.scale.y * p_gc.scale)
            },
            .texture = p_rm.getTextureCache().find("predator_weapon")->second
        }
    );

    Vector2f frameOffset = Vector2f(spriteSrc.w, spriteSrc.h);

    p_ecs.addComponent(
        playerWeapon,
        Animation
        {
            .infoMap =
            {
                {AnimationName::SHOOT, {0, 6, frameOffset, AnimationType::ONE_TIME}}
            },
            .name = AnimationName::SHOOT,
            .frameCounter = 0,
            .frameRatioMultiplier = 1.0f
        }
    );

    // Vector2 boxOffset = Vector2(0, 0);
    // Vector2 boxSize = Vector2(29, 13);

    // p_ecs.addComponent(
    //     playerWeapon,
    //     BoxCollider
    //     {
    //         .box = SDL_Rect
    //         {
    //             static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
    //             static_cast<int>((transform.position.y + boxOffset.y * p_gc.scale) * transform.scale.y),
    //             static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
    //             static_cast<int>(boxSize.y * transform.scale.y * p_gc.scale)
    //         },
    //         .offset = boxOffset,
    //         .isStatic = false,
    //         .debug = true
    //     }
    // );

    Vector2 bulletPositionOffset = Vector2(29 - 11, 3);

    p_ecs.addComponent(
        playerWeapon,
        WeaponProperties
        {
            .isAttacking = false,
            .attackSpeed = 5.0f,
            .bulletPositionOffset = bulletPositionOffset,
            .bulletSpeed = 1000.0f
        }
    );

    p_ecs.addComponent(
        playerWeapon,
        ControllableComponent
        {}
    );

    return playerWeapon;
}

Entity entityFactory::makeBullet(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Transform& p_initialTransform)
{
    Entity bullet = p_ecs.createEntity();

    // add components

    p_ecs.addComponent(
        bullet,
        RigidBody
        {
            .velocity = Vector2f(),
            .acceleration = Vector2f(),
            .gravity = Vector2f(0.0f, 0.0f),
            .friction = 0.0f,
            .maxVelocity = Vector2f(400.0f, 400.0f),
            .moveRemainder = Vector2f()
        }
    );

    p_ecs.addComponent(
        bullet,
        p_initialTransform
    );

    auto const& transform = p_ecs.getComponent<Transform>(bullet);
    SDL_Rect spriteSrc = SDL_Rect{0, 0, 16, 3};
    p_ecs.addComponent(
        bullet,
        Sprite
        {
            .offset = Vector2(spriteSrc.x, spriteSrc.y),
            .src = spriteSrc,
            .dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(spriteSrc.w * transform.scale.x * p_gc.scale),
                static_cast<int>(spriteSrc.h * transform.scale.y * p_gc.scale)
            },
            .texture = p_rm.getTextureCache().find("predator_bullet")->second
        }
    );

    Vector2f frameOffset = Vector2f(spriteSrc.w, spriteSrc.h);

    p_ecs.addComponent(
        bullet,
        Animation
        {
            .infoMap =
            {
                {AnimationName::BULLET, {0, 3, frameOffset, AnimationType::ONE_TIME}}
            },
            .name = AnimationName::BULLET,
            .frameCounter = 0,
            .frameRatioMultiplier = 1.0f
        }
    );

    Vector2 boxOffset = Vector2(spriteSrc.w - spriteSrc.h, 0);
    Vector2 boxSize = Vector2(spriteSrc.h, spriteSrc.h);

    p_ecs.addComponent(
        bullet,
        BoxCollider
        {
            .box = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + boxOffset.y * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(boxSize.y * transform.scale.y * p_gc.scale)
            },
            .offset = boxOffset,
            .isStatic = false,
            .debug = true
        }
    );

    p_ecs.addComponent(
        bullet,
        Damage
        {
            .value = 10.0f,
            .type = DamageType::PIERCE
        }
    );

    return bullet;
}

Entity entityFactory::makeDummy(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Vector2& p_pos)
{
    Entity dummy = p_ecs.createEntity();

    // add components

    p_ecs.addComponent(
        dummy,
        RigidBody
        {
            .velocity = Vector2f(),
            .acceleration = Vector2f(),
            .gravity = Vector2f(0.0f, 500.0f),
            .friction = 0.0f,
            .maxVelocity = Vector2f(0.0f, 400.0f),
            .moveRemainder = Vector2f()
        }
    );

    p_ecs.addComponent(
        dummy,
        Transform
        {
            .position = p_pos,
            .rotation = 0.0f,
            .rotationPoint = RotationPoint::CENTER,
            .scale = Vector2f(1.0f, 1.0f)
        }
    );

    auto const& transform = p_ecs.getComponent<Transform>(dummy);
    SDL_Rect spriteSrc = SDL_Rect{0, 0, 50, 50};
    p_ecs.addComponent(
        dummy,
        Sprite
        {
            .offset = Vector2(spriteSrc.x, spriteSrc.y),
            .src = spriteSrc,
            .dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(spriteSrc.w * transform.scale.x * p_gc.scale),
                static_cast<int>(spriteSrc.h * transform.scale.y * p_gc.scale)
            },
            .texture = p_rm.getTextureCache().find("dummy_sprite")->second
        }
    );

    Vector2f frameOffset = Vector2f(spriteSrc.w, spriteSrc.h);

    p_ecs.addComponent(
        dummy,
        Animation
        {
            .infoMap =
            {
                {AnimationName::IDLE, {0, 1, frameOffset, AnimationType::CONTINUOUS}},
            },
            .name = AnimationName::IDLE,
            .frameCounter = 0,
            .frameRatioMultiplier = 1.0f
        }
    );

    Vector2 boxOffset = Vector2(19, 7);
    Vector2 boxSize = Vector2(12, 40);

    p_ecs.addComponent(
        dummy,
        BoxCollider
        {
            .box = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + boxOffset.y * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(boxSize.y * transform.scale.y * p_gc.scale)
            },
            .offset = boxOffset,
            .isStatic = false,
            .groundCheck = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + (boxOffset.y + boxSize.y) * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(1 * transform.scale.y * p_gc.scale)
            },
            .debug = true
        }
    );

    float maxHealth = 100.0f;

    p_ecs.addComponent(
        dummy,
        Health
        {
            .value = maxHealth,
            .max = maxHealth
        }
    );

    return dummy;
}

Entity entityFactory::makeTile(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Vector2& p_pos)
{
    Entity tile = p_ecs.createEntity();

    // add components
    p_ecs.addComponent(
        tile,
        Transform
        {
            .position = p_pos,
            .rotation = 0.0f,
            .rotationPoint = RotationPoint::CENTER,
            .scale = Vector2f(1.0f, 1.0f)
        }
    );

    auto const& transform = p_ecs.getComponent<Transform>(tile);
    SDL_Rect spriteSrc = SDL_Rect{0, 0, 24, 24};
    p_ecs.addComponent(
        tile,
        Sprite
        {
            .offset = Vector2(spriteSrc.x, spriteSrc.y),
            .src = spriteSrc,
            .dst = SDL_Rect
            {
                static_cast<int>(transform.position.x * transform.scale.x),
                static_cast<int>(transform.position.y * transform.scale.y),
                static_cast<int>(spriteSrc.w * transform.scale.x * p_gc.scale),
                static_cast<int>(spriteSrc.h * transform.scale.y * p_gc.scale)
            },
            .texture = p_rm.getTextureCache().find("test_tiles")->second
        }
    );

    Vector2 boxOffset = Vector2(0, 0);
    Vector2 boxSize = Vector2(24, 24);

    p_ecs.addComponent(
        tile,
        BoxCollider
        {
            .box = SDL_Rect
            {
                static_cast<int>((transform.position.x + boxOffset.x * p_gc.scale) * transform.scale.x),
                static_cast<int>((transform.position.y + boxOffset.y * p_gc.scale) * transform.scale.y),
                static_cast<int>(boxSize.x * transform.scale.x * p_gc.scale),
                static_cast<int>(boxSize.y * transform.scale.y * p_gc.scale)
            },
            .offset = boxOffset,
            .isStatic = true,
            .debug = true
        }
    );

    return tile;
}