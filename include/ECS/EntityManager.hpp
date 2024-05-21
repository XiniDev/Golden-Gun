#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>

#include "Types.hpp"

class EntityManager
{
    private:
        std::uint32_t entityCount;
        std::queue<Entity> availableEntities;
	    std::array<Signature, MAX_ENTITIES> signatures;

    public:
        // constructors & destructors
        EntityManager()
            :entityCount(0), availableEntities(), signatures()
        {
            for (Entity entity = 0; entity < MAX_ENTITIES; entity++)
            {
                availableEntities.push(entity);
            }
        }

        // getters
        Signature getSignature(Entity p_entity)
        {
            if (p_entity >= MAX_ENTITIES)
                throw std::runtime_error("Cannot get signature with entity id: " + std::to_string(p_entity) + " which exceeds MAX_ENTITIES of " + std::to_string(MAX_ENTITIES));

            return signatures[p_entity];
        }

        // setters
        void setSignature(Entity p_entity, Signature p_signature)
        {
            if (p_entity >= MAX_ENTITIES)
                throw std::runtime_error("Cannot set signature with entity id: " + std::to_string(p_entity) + " which exceeds MAX_ENTITIES of " + std::to_string(MAX_ENTITIES));

            signatures[p_entity] = p_signature;
        }

        // logic
        Entity createEntity()
        {
            if (entityCount >= MAX_ENTITIES)
                throw std::runtime_error("Cannot create entity with entity id: " + std::to_string(entityCount) + " which exceeds MAX_ENTITIES of " + std::to_string(MAX_ENTITIES));

            Entity id = availableEntities.front();
            availableEntities.pop();
            entityCount++;

            return id;
        }

        void destroyEntity(Entity p_entity)
        {
            if (p_entity >= MAX_ENTITIES)
                throw std::runtime_error("Cannot destroy entity with entity id: " + std::to_string(p_entity) + " which exceeds MAX_ENTITIES of " + std::to_string(MAX_ENTITIES));

            signatures[p_entity].reset();
            availableEntities.push(p_entity);
            entityCount--;
        }
};