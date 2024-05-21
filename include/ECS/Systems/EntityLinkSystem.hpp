#pragma once

#include <iostream>

#include "ECS/System.hpp"

#include "ECS/Components/EntityLink.hpp"
#include "ECS/Components/Transform.hpp"

struct EntityLinkSystem : public System
{
    // constructors & destructors
    EntityLinkSystem()
    {}

    // logic

    void update(ECSManager& p_ecs, const GameConfig& p_gc, const ResourceManager& p_rm, float p_dt) override
    {
        for (auto const& entity: entities)
        {
            auto const& entityLink = p_ecs.getComponent<EntityLink>(entity);
            auto& transform = p_ecs.getComponent<Transform>(entity);

            auto const& linkedTransform = p_ecs.getComponent<Transform>(entityLink.mainEntity);

            transform.position = Vector2(static_cast<int>(linkedTransform.position.x + (entityLink.posOffset.x * p_gc.scale)),
                                         static_cast<int>(linkedTransform.position.y + (entityLink.posOffset.y * p_gc.scale)));

            // add facing direction & tweak the position in the future
        }
    }
};