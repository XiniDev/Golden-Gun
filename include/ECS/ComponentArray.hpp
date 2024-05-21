#pragma once

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>

#include "Types.hpp"

class IComponentArray
{
    public:
        virtual ~IComponentArray() = default;
        virtual void entityDestroyed(Entity p_entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
    private:
        std::array<T, MAX_ENTITIES> componentArray;
        std::unordered_map<Entity, size_t> entityToIndexMap;
        std::unordered_map<size_t, Entity> indexToEntityMap;
        size_t size;

    public:
        // getters
        T& getData(Entity p_entity)
        {
            if (entityToIndexMap.find(p_entity) == entityToIndexMap.end())
                throw std::runtime_error("Component not found for the entity with id: " + std::to_string(p_entity));

            // return a reference to the entity's component
            return componentArray[entityToIndexMap[p_entity]];
        }

        // logic
        void insertData(Entity p_entity, T p_component)
        {
            if (entityToIndexMap.find(p_entity) != entityToIndexMap.end())
                throw std::runtime_error("Component cannot be added to the same entity more than once. Entity has id: " + std::to_string(p_entity));

            // put new entry at the end and update maps
            size_t newIndex = size;
            entityToIndexMap[p_entity] = newIndex;
            indexToEntityMap[newIndex] = p_entity;
            componentArray[newIndex] = p_component;
            size++;
        }
        void removeData(Entity p_entity)
        {
            if (entityToIndexMap.find(p_entity) == entityToIndexMap.end())
                throw std::runtime_error("Component cannot be removed when it doesn't exist on the entity. Entity has id: " + std::to_string(p_entity));

            // copy element at end into deleted element's place to maintain density
            size_t indexOfRemovedEntity = entityToIndexMap[p_entity];
            size_t indexOfLastElement = size - 1;
            componentArray[indexOfRemovedEntity] = componentArray[indexOfLastElement];

            // update map to point to moved spot
            Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
            entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            entityToIndexMap.erase(p_entity);
            indexToEntityMap.erase(indexOfLastElement);

            size--;
        }

        void entityDestroyed(Entity p_entity) override
        {
            // remove the component from a destroyed entity
            if (entityToIndexMap.find(p_entity) != entityToIndexMap.end())
                removeData(p_entity);
        }
};