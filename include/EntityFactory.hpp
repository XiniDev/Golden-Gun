#pragma once

#include "Vector2.hpp"
#include "GameConfig.hpp"
#include "ResourceManager.hpp"

#include "ECS/ECSManager.hpp"

// components
#include "ECS/Components/Animation.hpp"
#include "ECS/Components/BoxCollider.hpp"
#include "ECS/Components/ControllableComponent.hpp"
#include "ECS/Components/Damage.hpp"
#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Health.hpp"
#include "ECS/Components/MoveProperties.hpp"
#include "ECS/Components/RigidBody.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/WeaponProperties.hpp"

namespace entityFactory
{
    Entity makePlayer(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm);
    Entity makePlayerWeapon(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, Entity p_linkedEntity);
    Entity makeBullet(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Transform& p_initialTransform);
    Entity makeDummy(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Vector2& p_pos);
    Entity makeTile(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, const Vector2& p_pos);
}